
#include "util/tc_progress_timer.h"

namespace tars
{

std::atomic<size_t> TC_ContinuousTimer::_seq = {0};

std::mutex TC_ContinuousTimer::_mutex;

unordered_set<shared_ptr<list<shared_ptr<TC_ContinuousTimer::RecordPoint>>>> TC_ContinuousTimer::_recordPointsList;

thread_local shared_ptr<list<shared_ptr<TC_ContinuousTimer::RecordPoint>>> TC_ContinuousTimer::_recordPoints;

void TC_ContinuousTimer::enable(bool open)
{
	_enable = open;
}
void TC_ContinuousTimer::add(const string &name)
{
	if(_enable) {
		shared_ptr<RecordPoint> rp = std::make_shared<RecordPoint>();
		rp->seq = ++_seq;
		rp->name = name;
		rp->us = TC_Common::now2us();

		if (!_recordPoints) {
			_recordPoints = std::make_shared<list<shared_ptr<RecordPoint>>>();
			std::lock_guard<std::mutex> lock(_mutex);
			_recordPointsList.insert(_recordPoints);
		}
		_recordPoints->push_back(rp);
	}
}

list<pair<string, int64_t>> TC_ContinuousTimer::statics()
{
	list<pair<string, int64_t>> result;

	std::lock_guard<std::mutex> lock(_mutex);

	map<size_t, shared_ptr<RecordPoint>> data;

	for (auto it = _recordPointsList.begin(); it != _recordPointsList.end(); ++it)
	{
		for(auto itr = (*it)->begin(); itr != (*it)->end(); ++itr)
		{
			data[(*itr)->seq] = *itr;
		}
	}

	ostringstream os;

	shared_ptr<RecordPoint> last;

	unordered_map<string, size_t> key;

	for(auto it = data.begin(); it != data.end(); ++it)
	{
		if(!last) {
			last = it->second;
			continue;
		}

		string name = TC_Common::outfill(last->name, ' ', 20) + " - " + TC_Common::outfill(it->second->name, ' ', 20);

		if(!key.count(name))
		{
			size_t size = key.size();
			key[name] = size;
			name = TC_Common::outfill(TC_Common::tostr(size), '0', 5, false) + " : " + name;
		}
		else
		{
			name = TC_Common::outfill(TC_Common::tostr(key[name]), '0', 5, false) + " : " + name;
		}

		result.push_back(std::make_pair(name, it->second->us - last->us));

		last = it->second;
	}

	return result;
}

void TC_ContinuousTimer::output()
{
	auto result = statics();

	for(auto it = result.begin(); it != result.end(); ++it)
	{
		cout << it->first << " : " << it->second << endl;
	}
}

void TC_ContinuousTimer::outputAvg()
{
	auto result = statics();

	map<string, pair<int64_t, int64_t>> data;

	for(auto it = result.begin(); it != result.end(); ++it)
	{
		data[it->first].first += it->second;
		data[it->first].second++;
	}

	for(auto it = data.begin(); it != data.end(); ++it)
	{
		if(it->second.second > 1 ) {
			cout << it->first << " : " << it->second.first << "/" << it->second.second << " "
			     << it->second.first / it->second.second << endl;
		}
	}
}

};

