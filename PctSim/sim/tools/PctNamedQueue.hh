//created by hujf@scnu.edu.cn @ 2021.10.25
#pragma once

#include <string>
#include <map>
#include "ProtectedQueue.hh"

/**
 * A protected queue that can be found via a global name.
 */
template <class T>
class NamedQueue : public ProtectedQueue<T>
{
	public:
		/// Initialize queue with name.
		NamedQueue(const std::string &name, unsigned int max_size = 0);
		virtual ~NamedQueue();

	private:
		NamedQueue(const std::string &name, unsigned int max_size, bool nolock);

	public:
		/// Get name of queue.
		std::string name() const { return m_name; }

	public:
		/// Find a named queue.
		static NamedQueue<T> *find(const std::string &name);

		/// Find or create a message queue.
		static NamedQueue<T> *create(const std::string &name, unsigned int max_size = 0);

	private:
		std::string m_name;

		// protect access to s_map;
		static boost::mutex s_map_mutex;

		// map from names to NamedQueue<T>*
		static std::map<std::string, NamedQueue<T> *> s_map;
};

	template <class T>
	NamedQueue<T>::NamedQueue(const std::string &name, unsigned int max_size)
: ProtectedQueue<T>(max_size),
	m_name(name)
{
	boost::mutex::scoped_lock lock(s_map_mutex);
	s_map[name] = this;
}

	template <class T>
	NamedQueue<T>::NamedQueue(const std::string &name, unsigned int max_size, bool nolock)
: ProtectedQueue<T>(max_size),
	m_name(name)
{
	s_map[name] = this;
}

	template <class T>
NamedQueue<T>::~NamedQueue()
{
	boost::mutex::scoped_lock lock(s_map_mutex);
	s_map.erase(m_name);
}

	template <class T>
NamedQueue<T> *NamedQueue<T>::find(const std::string &name)
{
	boost::mutex::scoped_lock lock(s_map_mutex);
	typename std::map<std::string, NamedQueue<T> *>::iterator it = s_map.find(name);
	if (it != s_map.end())
	{
		return (*it).second;
	}
	return 0;
}

	template <class T>
NamedQueue<T> *NamedQueue<T>::create(const std::string &name, unsigned int max_size)
{
	boost::mutex::scoped_lock lock(s_map_mutex);

	// first check if message queue already exists
	typename std::map<std::string, NamedQueue<T> *>::iterator it = s_map.find(name);
	if (it != s_map.end())
	{
		return (*it).second;
	}

	return new NamedQueue<T>(name, max_size, true);
}

template <class T>
std::map<std::string, NamedQueue<T> *> NamedQueue<T>::s_map;

template <class T>
boost::mutex NamedQueue<T>::s_map_mutex;

