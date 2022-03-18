#include "sys/epoll.h"
#include "msafd.h"

#include <unordered_map>
#include <unordered_set>
#include <cassert>
#include <mutex>
#include <iostream>
#include <algorithm>

using namespace std;

#define ARRAY_COUNT(a) (sizeof(a) / (sizeof((a)[0])))

#define EPOLL__EVENT_MASK 0xffff

class epoll_port_data_t;
struct epoll_sock_data_t;
struct epoll_op_t;

/* State associated with a AFD_POLL request. */
struct epoll_op_t
{
    OVERLAPPED _overlapped;
    AFD_POLL_INFO _poll_info;
    uint32_t _generation;
    epoll_sock_data_t *_sock_data;

    epoll_op_t(epoll_sock_data_t *sock_data, uint32_t afd_events);
    ~epoll_op_t();
};

/* State associated with a socket that is registered to the epoll port. */
struct epoll_sock_data_t
{
    SOCKET _sock;
    SOCKET _base_sock;
    SOCKET _peer_sock;
    uint32_t _registered_events;
    uint32_t _op_generation;
    uint64_t _user_data;
    int32_t _op_count;

    epoll_sock_data_t(SOCKET sock, SOCKET base_sock, SOCKET peer_sock);

    int submit();
};

/* State associated with a epoll handle. */
class epoll_port_data_t
{
protected:
    HANDLE _iocp;
    SOCKET _peer_sockets[ARRAY_COUNT(AFD_PROVIDER_IDS)];
    unordered_map<int, epoll_sock_data_t *> _sock_data_tree;
    unordered_set<epoll_sock_data_t *> _attn_list;
	std::mutex _mutex;

public:
	epoll_port_data_t();
    HANDLE getHandle() { return _iocp; }
	int epoll_add(SOCKET sock, struct epoll_event *ev);
	int epoll_mod(SOCKET sock, struct epoll_event *ev);
	int epoll_del(SOCKET sock, struct epoll_event *ev);
	int epoll_close();
    int epoll_wait(OVERLAPPED_ENTRY *entries, ULONG count, struct epoll_event *events);
    void submit()
    {
        std::lock_guard<std::mutex> lck (_mutex);
        
        for(auto entry : _attn_list)
        {
            if(entry->_op_count == 0) {
            // printf("before epoll_wait submit: %d\n", entry->_op_count);
                //no focus any event, then submit keep attn, otherwise cause op leak
                entry->submit();
            }
        }

        _attn_list.clear();
    } 
protected:
    bool find(epoll_sock_data_t *sock_data)
    {
	    auto it = _sock_data_tree.find(sock_data->_sock);
	    if (it != _sock_data_tree.end()) {
		    return it->second == sock_data;
	    }
	    return false;
    }

    void remove(epoll_sock_data_t *sock_data)
    {
        _sock_data_tree.erase(sock_data->_sock);
        _attn_list.erase(sock_data);
    }
    void add(epoll_sock_data_t *sock_data)
    {
        _sock_data_tree[sock_data->_sock] = sock_data;
    }
    void remove_attn(epoll_sock_data_t *sock_data)
    {
        _attn_list.erase(sock_data);
    }
    void add_attn(epoll_sock_data_t *sock_data)
    {
        _attn_list.insert(sock_data);
    }

    epoll_sock_data_t *get(SOCKET sock)
    {
        auto it = _sock_data_tree.find(sock);
        if (it == _sock_data_tree.end())
        {
            /* Socket has not been registered with epoll instance. */
            SetLastError(ERROR_NOT_FOUND);
            return NULL;
        }
        return it->second;
    }

    SOCKET create_peer_socket(WSAPROTOCOL_INFOW *protocol_info);
    SOCKET get_peer_socket(WSAPROTOCOL_INFOW *protocol_info);
    
};

static int epoll__initialize();
static int epoll__afd_poll(SOCKET socket, AFD_POLL_INFO *info, OVERLAPPED *overlapped);
static int epoll__ntstatus_to_winsock_error(NTSTATUS status);

static int epoll__initialized = 0;
static PNTDEVICEIOCONTROLFILE pNtDeviceIoControlFile;

epoll_op_t::epoll_op_t(epoll_sock_data_t *sock_data, uint32_t afd_events)
{
    _generation = sock_data->_op_generation + 1;

    memset(&_overlapped, 0, sizeof _overlapped);
    _poll_info.Exclusive         = FALSE;
    _poll_info.NumberOfHandles   = 1;
    _poll_info.Timeout.QuadPart  = INT64_MAX;
    _poll_info.Handles[0].Handle = (HANDLE)sock_data->_base_sock;
    _poll_info.Handles[0].Status = 0;
    _poll_info.Handles[0].Events = afd_events;       

    _sock_data = sock_data;
    _sock_data->_op_count++;

    // printf("submit new: %p, %d\n", this, _sock_data->_op_count);
 
}

epoll_op_t::~epoll_op_t()
{
    // printf("~epoll_op_t: %p, %d\n", this, _sock_data->_op_count  );

    _sock_data->_op_count--; 
    assert(_sock_data->_op_count >= 0);
}

epoll_sock_data_t::epoll_sock_data_t(SOCKET sock, SOCKET base_sock, SOCKET peer_sock)
{
    _sock              = sock;
    _base_sock         = base_sock;
    _op_generation     = 0;
    _registered_events = 0;
    _user_data         = 0;
    _peer_sock         = peer_sock;
    _op_count          = 0;
}

int epoll_sock_data_t::submit()
{
    DWORD afd_events = AFD_POLL_ABORT | AFD_POLL_DISCONNECT | AFD_POLL_CONNECT_FAIL | AFD_POLL_LOCAL_CLOSE;

    if (_registered_events & (EPOLLIN | EPOLLRDNORM))
    {
        afd_events |= AFD_POLL_RECEIVE | AFD_POLL_ACCEPT ;
    }
    if (_registered_events & (EPOLLIN | EPOLLRDBAND))
    {
        afd_events |= AFD_POLL_RECEIVE_EXPEDITED;
    }
    if (_registered_events & (EPOLLOUT | EPOLLWRNORM | EPOLLRDBAND))
    {
        afd_events |= AFD_POLL_SEND | AFD_POLL_CONNECT;
    }

    epoll_op_t* op = new epoll_op_t(this, afd_events);

    DWORD result = epoll__afd_poll(_peer_sock, &op->_poll_info, &op->_overlapped);
    if (result != 0)
    {
        DWORD error = WSAGetLastError();
        if (error != WSA_IO_PENDING)
        {
            /* If this happens an error happened and no overlapped operation was */
            /* started. */
            delete op;
            op = NULL;
            return -1;
        }
    }

    _op_generation    = op->_generation;

    return 0;
}

//////////////////////////////////////////////////////////////////////
epoll_port_data_t::epoll_port_data_t()
{
    _iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    if (_iocp == INVALID_HANDLE_VALUE)
    {
        return;
    }

    memset(&_peer_sockets, 0, sizeof _peer_sockets);
}

SOCKET epoll_port_data_t::get_peer_socket(WSAPROTOCOL_INFOW *protocol_info)
{
    int index = -1;

    for (int i = 0; i < ARRAY_COUNT(AFD_PROVIDER_IDS); i++)
    {
        if (memcmp((void *)&protocol_info->ProviderId, (void *)&AFD_PROVIDER_IDS[i], sizeof protocol_info->ProviderId) == 0)
        {
            index = i;
        }
    }

    /* Check if the protocol uses an msafd socket. */
    if (index < 0)
    {
        SetLastError(ERROR_NOT_SUPPORTED);
        return INVALID_SOCKET;
    }

    /* If we didn't (try) to create a peer socket yet, try to make one. Don't */
    /* try again if the peer socket creation failed earlier for the same */
    /* protocol. */
    SOCKET peer_socket = _peer_sockets[index];
    if (peer_socket == 0)
    {
        peer_socket          = create_peer_socket(protocol_info);
        _peer_sockets[index] = peer_socket;
    }

    return peer_socket;
}

SOCKET epoll_port_data_t::create_peer_socket(WSAPROTOCOL_INFOW *protocol_info)
{
    SOCKET sock = WSASocketW(protocol_info->iAddressFamily, protocol_info->iSocketType, protocol_info->iProtocol, protocol_info, 0, WSA_FLAG_OVERLAPPED);
    if (sock == INVALID_SOCKET)
    {
        return INVALID_SOCKET;
    }

    if (!SetHandleInformation((HANDLE)sock, HANDLE_FLAG_INHERIT, 0))
    {
        goto error;
    };

    if (CreateIoCompletionPort((HANDLE)sock, _iocp, 0, 0) == NULL)
    {
        goto error;
    }

    return sock;

error:
    closesocket(sock);
    return INVALID_SOCKET;
}

int epoll_port_data_t::epoll_add(SOCKET sock, struct epoll_event *ev)
{
    std::lock_guard<std::mutex> lck (_mutex);

    epoll_sock_data_t *sock_data = get(sock);
    if(sock_data != NULL)
    {
        SetLastError(ERROR_ALREADY_EXISTS);
        return -1;
    }

    WSAPROTOCOL_INFOW protocol_info;
    DWORD bytes;

    /* Try to obtain a base handle for the socket, so we can bypass LSPs */
    /* that get in the way if we want to talk to the kernel directly. If */
    /* it fails we try if we work with the original socket. Note that on */
    /* windows XP/2k3 this will always fail since they don't support the */
    /* SIO_BASE_HANDLE ioctl. */
    SOCKET base_sock = sock;
    WSAIoctl(sock, SIO_BASE_HANDLE, NULL, 0, &base_sock, sizeof base_sock, &bytes, NULL, NULL);

    /* Obtain protocol information about the socket. */
    int len = sizeof protocol_info;
    if (getsockopt(base_sock, SOL_SOCKET, SO_PROTOCOL_INFOW, (char *)&protocol_info, &len) != 0)
    {
        return -1;
    }

    SOCKET peer_sock = get_peer_socket(&protocol_info);
    if (peer_sock == INVALID_SOCKET)
    {
        return -1;
    }

    sock_data = new epoll_sock_data_t(sock, base_sock, peer_sock);
    if (sock_data == NULL)
    {
        SetLastError(ERROR_OUTOFMEMORY);
        return -1;
    }
    // printf("add new, %d\n", sock_data->_op_count);

    add(sock_data);

    sock_data->_registered_events = ev->events | EPOLLERR | EPOLLHUP;
    sock_data->_user_data         = ev->data.u64;

    return sock_data->submit();    
}

int epoll_port_data_t::epoll_mod(SOCKET sock, struct epoll_event *ev)
{
    {
        std::lock_guard<std::mutex> lck (_mutex);

        epoll_sock_data_t *sock_data = get(sock);

        if(sock_data != NULL)
        {
            sock_data->_registered_events = ev->events | EPOLLERR | EPOLLHUP;
            sock_data->_user_data         = ev->data.u64;
            return sock_data->submit();  
        }
    }
    // LOG_CONSOLE_DEBUG << endl;
    return epoll_add(sock, ev);
}

int epoll_port_data_t::epoll_del(SOCKET sock, struct epoll_event *ev)
{
    std::lock_guard<std::mutex> lck (_mutex);

    // printf("del new\n");

    epoll_sock_data_t *sock_data = get(sock);
    if(sock_data == NULL)
    {
        SetLastError(ERROR_NOT_FOUND);
        return -1;
    }

    /* Remove from attention list. */
    remove(sock_data);

    return 0;
    // 以下代码不能有, 否则短链接模式下句柄复用会有问题！！！！
    // sock_data->_registered_events = ev->events | EPOLLERR | EPOLLHUP;
    // sock_data->_user_data         = ev->data.u64;

    // return sock_data->submit();    
}

int epoll_port_data_t::epoll_close()
{
    std::lock_guard<std::mutex> lck (_mutex);
 
    /* Close all peer sockets. This will make all pending ops return. */
    for (int i = 0; i < ARRAY_COUNT(_peer_sockets); i++)
    {
        SOCKET peer_sock = _peer_sockets[i];
        if (peer_sock != 0 && peer_sock != INVALID_SOCKET)
        {
            closesocket(peer_sock);

            _peer_sockets[i] = 0;
        }
    }

    /* Remove all entries from the socket_state tree. */
    for(auto entry : _sock_data_tree)
    {
        entry.second->submit();
    }

    while(!_sock_data_tree.empty())
    {
        OVERLAPPED_ENTRY entries[64];
        ULONG count;

        DWORD result = GetQueuedCompletionStatusEx(_iocp, entries, ARRAY_COUNT(entries), &count, 0, FALSE);
        if (!result)
        {
            DWORD error = GetLastError();
            return -1;
        }

        for(ULONG i = 0; i < count; i++)
        {
            epoll_op_t *op = CONTAINING_RECORD(entries[i].lpOverlapped, epoll_op_t, _overlapped);
            epoll_sock_data_t *sock_data = op->_sock_data;

            if(!find(sock_data))
            {
                remove(sock_data);
            }

            delete op;
            if(sock_data->_op_count == 0)
                delete sock_data;
        }

        if(count == 0)
            break;
    }

    /* Close the I/O completion port. */
    CloseHandle(_iocp);

	return 0;
}

int epoll_port_data_t::epoll_wait(OVERLAPPED_ENTRY *entries, ULONG count, struct epoll_event *events)
{
    std::lock_guard<std::mutex> lck (_mutex);

    int num_events = 0;

    /* Successfully dequeued overlappeds. */
    for (ULONG i = 0; i < count; i++)
    {
        epoll_op_t *op = CONTAINING_RECORD(entries[i].lpOverlapped, epoll_op_t, _overlapped);
        epoll_sock_data_t *sock_data = op->_sock_data;
        
        if (op->_generation < sock_data->_op_generation)
        {
     //       cout << "op superseded" << endl;
            delete op;
            continue;
        }
        

        /* Dequeued the most recent op. Reset generation and submitted_events. */
        sock_data->_op_generation = 0;

        /* Check if this op was associated with a socket that was removed */
        /* with EPOLL_CTL_DEL. */
        if(!find(sock_data))
        {
            delete op;
            if(sock_data->_op_count == 0)
                delete sock_data;
            continue;
        }

        /* Check for error. */
        if (!NT_SUCCESS(entries[i].lpOverlapped->Internal))
        {
            struct epoll_event *ev = events + num_events;
            ev->data.u64 = sock_data->_user_data;
            ev->events = EPOLLERR;
	        num_events++;
            continue;
        }

        DWORD afd_events;

        if (op->_poll_info.NumberOfHandles == 0)
        {
            /* NumberOfHandles can be zero if this poll operation was canceled */
            /* due to a more recent exclusive poll operation. */
            afd_events = 0;
        }
        else
        {
            afd_events = op->_poll_info.Handles[0].Events;
        }

        // cout << "wait ok, handles:" << op->_poll_info.NumberOfHandles << ", events:" << afd_events << endl;

        /* Check for a closed socket. */
        if (afd_events & AFD_POLL_LOCAL_CLOSE)
        {
            remove(sock_data);
            delete op;
            if(sock_data->_op_count == 0)
                delete sock_data;
            continue;
        }

        delete op;

 //       int registered_events = sock_data->_registered_events;
        int reported_events = 0;
        /* Convert afd events to epoll events. */
        if (afd_events & (AFD_POLL_RECEIVE | AFD_POLL_ACCEPT))
        {
            reported_events |= EPOLLIN;
        }
        if (afd_events & AFD_POLL_RECEIVE_EXPEDITED)
        {
            reported_events |= (EPOLLIN | EPOLLRDBAND);
        }
        if (afd_events & AFD_POLL_SEND)
        {
            reported_events |= (EPOLLOUT);
        }
        if ((afd_events & AFD_POLL_DISCONNECT) && !(afd_events & AFD_POLL_ABORT))
        {
            reported_events |= (EPOLLRDHUP | EPOLLIN | EPOLLRDNORM | EPOLLRDBAND);
        }
        if (afd_events & AFD_POLL_ABORT)
        {
            reported_events |= EPOLLHUP | EPOLLERR;
        }
        if (afd_events & AFD_POLL_CONNECT)
        {
            reported_events |= (EPOLLOUT | EPOLLWRNORM | EPOLLWRBAND);
        }
        if (afd_events & AFD_POLL_CONNECT_FAIL)
        {
            reported_events |= EPOLLERR;
        }

        //keep attn
        sock_data->_registered_events &= ~EPOLLOUT;
        add_attn(sock_data); 
        
        /* Don't report events that the user didn't specify. */
        // reported_events &= registered_events;
        // if(reported_events & EPOLLOUT)
        //     cout << "reported_events EPOLLOUT:" << reported_events << endl;

        if (reported_events)
        {
            struct epoll_event *ev = events + num_events;
            ev->data.u64           = sock_data->_user_data;
            ev->events             = reported_events;
	        num_events++;
        }
    }
    return num_events;
}

///////////////////////////////////////////////////////////////////////////////////        
epoll_t epoll_create(int size)
{
    /* If necessary, do global initialization first. This is totally not */
    /* thread-safe at the moment. */
    if (!epoll__initialized)
    {
        if (epoll__initialize() < 0)
            return NULL;
        epoll__initialized = 1;
    }

    epoll_port_data_t *port_data = new epoll_port_data_t();
    if (port_data == NULL)
    {
        SetLastError(ERROR_OUTOFMEMORY);
        return NULL;
    }

    return (epoll_t)port_data;
}

int epoll_ctl(epoll_t port_handle, int op, SOCKET sock, struct epoll_event *ev)
{
    epoll_port_data_t *port_data = (epoll_port_data_t *)port_handle;

    switch (op)
    {
        case EPOLL_CTL_ADD:
        {
            return port_data->epoll_add(sock, ev);
        }
        case EPOLL_CTL_MOD:
        {
            return port_data->epoll_mod(sock, ev);
        }
        case EPOLL_CTL_DEL:
        {
            return port_data->epoll_del(sock, ev);
        }

        default:
            WSASetLastError(WSAEINVAL);
            return -1;
    }
}

int epoll_wait(epoll_t port_handle, struct epoll_event *events, int maxevents, int timeout)
{
    DWORD gqcs_timeout;

    /* Compute the timeout for GetQueuedCompletionStatus, and the wait end */
    /* time, if the user specified a timeout other than zero or infinite. */
    if (timeout >= 0)
    {
        gqcs_timeout = (DWORD)timeout;
    }
    else
    {
        gqcs_timeout = INFINITE;
    }

    epoll_port_data_t *port_data = (epoll_port_data_t *)port_handle;

    port_data->submit();

    OVERLAPPED_ENTRY entries[64];

    /* Compute how much overlapped entries can be dequeued at most. */
    int tmp_count = ARRAY_COUNT(entries);
    DWORD max_entries = std::min(tmp_count, maxevents);
    ULONG count = 0;

    DWORD result = GetQueuedCompletionStatusEx(port_data->getHandle(), entries, max_entries, &count, gqcs_timeout, TRUE);

    if (!result)
    {
        DWORD error = GetLastError();
        if (error == WAIT_TIMEOUT)
        {
        //    printf("%d, GetQueuedCompletionStatusEx:%d\n", std::this_thread::get_id() , count);

            return 0;
        }
        else
        {
            return -1;
        }
    }

    return port_data->epoll_wait(entries, count, events);
}

int epoll_close(epoll_t port_handle)
{
    epoll_port_data_t *port_data = (epoll_port_data_t *)port_handle;

    port_data->epoll_close();

    /* Finally, remove the port data. */
    delete port_data;
    port_data = NULL;

    return 0;
}

int epoll__initialize()
{
    HMODULE ntdll;
    int r;
    WSADATA wsa_data;

    r = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (r != 0)
        return -1;

    ntdll = LoadLibraryW(L"ntdll.dll");
    if (ntdll == NULL)
        return -1;

    pNtDeviceIoControlFile = (PNTDEVICEIOCONTROLFILE)GetProcAddress(ntdll, "NtDeviceIoControlFile");
    if (pNtDeviceIoControlFile == NULL)
        return -1;

    return 0;
}

int epoll__afd_poll(SOCKET socket, AFD_POLL_INFO *info, OVERLAPPED *overlapped)
{
    IO_STATUS_BLOCK iosb;
    IO_STATUS_BLOCK *iosb_ptr;
    HANDLE event = NULL;
    void *apc_context;
    NTSTATUS status;
    DWORD error;

    if (overlapped != NULL)
    {
        /* Overlapped operation. */
        iosb_ptr = (IO_STATUS_BLOCK *)&overlapped->Internal;
        event    = overlapped->hEvent;

        /* Do not report iocp completion if hEvent is tagged. */
        if ((uintptr_t)event & 1)
        {
            event       = (HANDLE)((uintptr_t)event & ~(uintptr_t)1);
            apc_context = NULL;
        }
        else
        {
            apc_context = overlapped;
        }
    }
    else
    {
        /* Blocking operation. */
        iosb_ptr = &iosb;
        event    = CreateEvent(NULL, FALSE, FALSE, NULL);
        if (event == NULL)
        {
            return SOCKET_ERROR;
        }
        apc_context = NULL;
    }

    iosb_ptr->Status = STATUS_PENDING;
    status = pNtDeviceIoControlFile((HANDLE)socket,
                                    event,
                                    NULL,
                                    apc_context,
                                    iosb_ptr,
                                    IOCTL_AFD_POLL,
                                    info,
                                    sizeof *info,
                                    info,
                                    sizeof *info);

    if (overlapped == NULL)
    {
        /* If this is a blocking operation, wait for the event to become */
        /* signaled, and then grab the real status from the io status block. */
        if (status == STATUS_PENDING)
        {
            DWORD r = WaitForSingleObject(event, INFINITE);

            if (r == WAIT_FAILED)
            {
                DWORD saved_error = GetLastError();
                CloseHandle(event);
                WSASetLastError(saved_error);
                return SOCKET_ERROR;
            }

            status = iosb.Status;
        }

        CloseHandle(event);
    }

    switch (status)
    {
        case STATUS_SUCCESS:
            error = ERROR_SUCCESS;
            break;

        case STATUS_PENDING:
            error = WSA_IO_PENDING;
            break;

        default:
            error = epoll__ntstatus_to_winsock_error(status);
            break;
    }

    WSASetLastError(error);

    if (error == ERROR_SUCCESS)
    {
        return 0;
    }
    else
    {
        return SOCKET_ERROR;
    }
}

int epoll__ntstatus_to_winsock_error(NTSTATUS status)
{
    switch (status)
    {
        case STATUS_SUCCESS:
            return ERROR_SUCCESS;

        case STATUS_PENDING:
            return ERROR_IO_PENDING;

        case STATUS_INVALID_HANDLE:
        case STATUS_OBJECT_TYPE_MISMATCH:
            return WSAENOTSOCK;

        case STATUS_INSUFFICIENT_RESOURCES:
        case STATUS_PAGEFILE_QUOTA:
        case STATUS_COMMITMENT_LIMIT:
        case STATUS_WORKING_SET_QUOTA:
        case STATUS_NO_MEMORY:
        case STATUS_CONFLICTING_ADDRESSES:
        case STATUS_QUOTA_EXCEEDED:
        case STATUS_TOO_MANY_PAGING_FILES:
        case STATUS_REMOTE_RESOURCES:
        case STATUS_TOO_MANY_ADDRESSES:
            return WSAENOBUFS;

        case STATUS_SHARING_VIOLATION:
        case STATUS_ADDRESS_ALREADY_EXISTS:
            return WSAEADDRINUSE;

        case STATUS_LINK_TIMEOUT:
        case STATUS_IO_TIMEOUT:
        case STATUS_TIMEOUT:
            return WSAETIMEDOUT;

        case STATUS_GRACEFUL_DISCONNECT:
            return WSAEDISCON;

        case STATUS_REMOTE_DISCONNECT:
        case STATUS_CONNECTION_RESET:
        case STATUS_LINK_FAILED:
        case STATUS_CONNECTION_DISCONNECTED:
        case STATUS_PORT_UNREACHABLE:
        case STATUS_HOPLIMIT_EXCEEDED:
            return WSAECONNRESET;

        case STATUS_LOCAL_DISCONNECT:
        case STATUS_TRANSACTION_ABORTED:
        case STATUS_CONNECTION_ABORTED:
            return WSAECONNABORTED;

        case STATUS_BAD_NETWORK_PATH:
        case STATUS_NETWORK_UNREACHABLE:
        case STATUS_PROTOCOL_UNREACHABLE:
            return WSAENETUNREACH;

        case STATUS_HOST_UNREACHABLE:
            return WSAEHOSTUNREACH;

        case STATUS_CANCELLED:
        case STATUS_REQUEST_ABORTED:
            return WSAEINTR;

        case STATUS_BUFFER_OVERFLOW:
        case STATUS_INVALID_BUFFER_SIZE:
            return WSAEMSGSIZE;

        case STATUS_BUFFER_TOO_SMALL:
        case STATUS_ACCESS_VIOLATION:
            return WSAEFAULT;

        case STATUS_DEVICE_NOT_READY:
        case STATUS_REQUEST_NOT_ACCEPTED:
            return WSAEWOULDBLOCK;

        case STATUS_INVALID_NETWORK_RESPONSE:
        case STATUS_NETWORK_BUSY:
        case STATUS_NO_SUCH_DEVICE:
        case STATUS_NO_SUCH_FILE:
        case STATUS_OBJECT_PATH_NOT_FOUND:
        case STATUS_OBJECT_NAME_NOT_FOUND:
        case STATUS_UNEXPECTED_NETWORK_ERROR:
            return WSAENETDOWN;

        case STATUS_INVALID_CONNECTION:
            return WSAENOTCONN;

        case STATUS_REMOTE_NOT_LISTENING:
        case STATUS_CONNECTION_REFUSED:
            return WSAECONNREFUSED;

        case STATUS_PIPE_DISCONNECTED:
            return WSAESHUTDOWN;

        case STATUS_INVALID_ADDRESS:
        case STATUS_INVALID_ADDRESS_COMPONENT:
            return WSAEADDRNOTAVAIL;

        case STATUS_NOT_SUPPORTED:
        case STATUS_NOT_IMPLEMENTED:
            return WSAEOPNOTSUPP;

        case STATUS_ACCESS_DENIED:
            return WSAEACCES;

        default:
            if ((status & (FACILITY_NTWIN32 << 16)) == (FACILITY_NTWIN32 << 16) &&
                (status & (ERROR_SEVERITY_ERROR | ERROR_SEVERITY_WARNING)))
            {
                /* It's a windows error that has been previously mapped to an */
                /* ntstatus code. */
                return (DWORD)(status & 0xffff);
            }
            else
            {
                /* The default fallback for unmappable ntstatus codes. */
                return WSAEINVAL;
            }
    }
}
