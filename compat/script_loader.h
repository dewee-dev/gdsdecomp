
#pragma once
#include "compat/resource_loader_compat.h"

class ResourceFormatGDScriptLoader : public CompatFormatLoader {
	GDCLASS(ResourceFormatGDScriptLoader, CompatFormatLoader);

public:
	virtual Ref<Resource> load(const String &p_path, const String &p_original_path = "", Error *r_error = nullptr, bool p_use_sub_threads = false, float *r_progress = nullptr, CacheMode p_cache_mode = CACHE_MODE_REUSE) override;
	virtual void get_recognized_extensions(List<String> *p_extensions) const override;
	virtual bool handles_type(const String &p_type) const override;
	virtual String get_resource_type(const String &p_path) const override;

	static Ref<Script> _set_script(const String &p_path, ResourceInfo::LoadType p_type, const String &script_text);
	virtual Ref<Resource> custom_load(const String &p_path, const String &p_original_path, ResourceInfo::LoadType p_type, Error *r_error = nullptr, bool use_threads = true, ResourceFormatLoader::CacheMode p_cache_mode = CACHE_MODE_REUSE) override;
	virtual Ref<ResourceInfo> get_resource_info(const String &p_path, Error *r_error) const override;
	virtual bool handles_fake_load() const override { return false; }
};
