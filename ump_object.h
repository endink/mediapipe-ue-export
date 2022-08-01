#pragma once

#include "ump_shared.h"

class IExtDummy {};

using UmpCustomDtor = std::function<void(IUmpObject* obj)>;

template<typename TBase, typename TExt = IExtDummy>
class UmpObject : public TBase, public TExt
{
protected:
	virtual ~UmpObject() override {}

public:
	UmpObject() : _ref_count(1) {}
	UmpObject(UmpCustomDtor& dtor) : _ref_count(1), _dtor(dtor) {}

	// non copyable
	UmpObject(const UmpObject&) = delete;
	UmpObject(UmpObject&&) = delete;
	UmpObject& operator=(const UmpObject&) = delete;
	UmpObject& operator=(const UmpObject&&) = delete;

	virtual void Release() override { ReleaseImpl(); }
	virtual void AddRef() override { AddRefImpl(); }

	inline void log(EUmpVerbosity verbosity, const char* msg) const { if (_ump_log) { _ump_log->Println(verbosity, msg); } }
	inline void log_e(const char* msg) const { log(EUmpVerbosity::Error, msg); }
	inline void log_w(const char* msg) const { log(EUmpVerbosity::Warning, msg); }
	inline void log_i(const char* msg) const { log(EUmpVerbosity::Info, msg); }
	inline void log_d(const char* msg) const { log(EUmpVerbosity::Debug, msg); }
	inline void log_e(const std::string& msg) const { log(EUmpVerbosity::Error, *msg); }
	inline void log_w(const std::string& msg) const { log(EUmpVerbosity::Warning, *msg); }
	inline void log_i(const std::string& msg) const { log(EUmpVerbosity::Info, *msg); }
	inline void log_d(const std::string& msg) const { log(EUmpVerbosity::Debug, *msg); }

protected:
	inline int ReleaseImpl()
	{
		const int n = _ref_count.fetch_sub(1); // TODO: better memory_order?
		if (n == 1)
		{
			if (_dtor)
				_dtor(static_cast<IUmpObject*>(this));
			else
				delete this;
		}
		return n;
	}

	inline int AddRefImpl()
	{
		return _ref_count.fetch_add(1); // TODO: better memory_order?
	}

protected:
	std::atomic<int> _ref_count;
	UmpCustomDtor _dtor;
};
