#pragma once

#include "ump_object.h"

using UmpContextBase = UmpObject<IUmpContext>;

class UmpContext : public UmpContextBase
{
public:
	UmpContext();
	virtual ~UmpContext() override;

	virtual void SetLog(class IUmpLog* log) override;
	virtual void SetResourceDir(const char* resource_dir) override;
	virtual class IUmpPipeline* CreatePipeline() override;
};
