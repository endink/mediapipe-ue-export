#include "ump_context.h"
#include "ump_pipeline.h"

ABSL_DECLARE_FLAG(std::string, resource_root_dir);

IUmpLog* _ump_log = nullptr;

UmpContext::UmpContext() {
	log_d("+UmpContext");
}

UmpContext::~UmpContext() {
	log_d("~UmpContext");
}

void UmpContext::SetLog(IUmpLog* log) {
	_ump_log = log;
}

void UmpContext::SetResourceDir(const char* resource_dir) {
	log_i(strf("SetResourceDir: %s", resource_dir));
	absl::SetFlag(&FLAGS_resource_root_dir, resource_dir);
}

IUmpPipeline* UmpContext::CreatePipeline() {
	return new UmpPipeline();
}

IUmpContext* UmpCreateContext() {
	return new UmpContext();
}
