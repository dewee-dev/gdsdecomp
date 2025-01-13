#ifndef IMPORT_EXPORTER_H
#define IMPORT_EXPORTER_H

#include "compat/resource_import_metadatav2.h"
#include "exporters/export_report.h"
#include "import_info.h"
#include "utility/godotver.h"

#include "editor/gdre_progress.h"

#include "core/object/object.h"
#include "core/object/ref_counted.h"

class ImportExporter;

class ImportExporterReport : public RefCounted {
	GDCLASS(ImportExporterReport, RefCounted)
	friend class ImportExporter;
	bool had_encryption_error = false;
	bool godotsteam_detected = false;
	bool exported_scenes = false;
	int session_files_total = 0;
	String log_file_location;
	Vector<String> decompiled_scripts;
	Vector<String> failed_scripts;
	String translation_export_message;
	Vector<Ref<ExportReport>> lossy_imports;
	Vector<Ref<ExportReport>> rewrote_metadata;
	Vector<Ref<ExportReport>> failed_rewrite_md;
	Vector<Ref<ExportReport>> failed_rewrite_md5;
	Vector<Ref<ExportReport>> failed;
	Vector<Ref<ExportReport>> success;
	Vector<Ref<ExportReport>> not_converted;
	Vector<String> failed_plugin_cfg_create;
	Vector<String> failed_gdnative_copy;
	Vector<String> unsupported_types;
	Ref<GodotVer> ver;
	// TODO: add the rest of the options
	bool opt_lossy = true;

public:
	void set_ver(String ver);
	String get_ver();
	void set_lossy_opt(bool lossy) {
		opt_lossy = lossy;
	}
	Dictionary get_totals();
	Dictionary get_unsupported_types();

	Dictionary get_session_notes();
	String get_totals_string();
	Dictionary get_report_sections();
	String get_report_string();
	String get_editor_message_string();
	String get_detected_unsupported_resource_string();

	String get_session_notes_string();

	String get_log_file_location();
	Vector<String> get_decompiled_scripts();
	Vector<String> get_failed_scripts();
	String get_translation_export_message();
	TypedArray<ImportInfo> get_lossy_imports() const;
	TypedArray<ImportInfo> get_rewrote_metadata() const;
	TypedArray<ImportInfo> get_failed_rewrite_md() const;
	TypedArray<ImportInfo> get_failed_rewrite_md5() const;
	TypedArray<ImportInfo> get_failed() const;
	TypedArray<ImportInfo> get_successes() const;
	TypedArray<ImportInfo> get_not_converted() const;
	Vector<String> get_failed_plugin_cfg_create() const;
	Vector<String> get_failed_gdnative_copy() const;

	void print_report();
	ImportExporterReport() {
		set_ver("0.0.0");
	}
	ImportExporterReport(String ver) {
		set_ver(ver);
	}

protected:
	static void _bind_methods();
};

class ImportExporter : public RefCounted {
	GDCLASS(ImportExporter, RefCounted)
	bool opt_bin2text = true;
	bool opt_export_textures = true;
	bool opt_export_samples = true;
	bool opt_export_ogg = true;
	bool opt_export_mp3 = true;
	bool opt_lossy = true;
	bool opt_export_jpg = true;
	bool opt_export_webp = true;
	bool opt_rewrite_imd_v2 = true;
	bool opt_rewrite_imd_v3 = true;
	bool opt_decompile = true;
	bool opt_only_decompile = false;
	bool opt_write_md5_files = true;
	bool opt_multi_thread = true;
	std::atomic<int> last_completed = 0;
	std::atomic<bool> cancelled = false;

	struct ExportToken {
		Ref<ImportInfo> iinfo;
		Ref<ExportReport> report;
		String output_dir;
		bool supports_multithread;
		bool opt_rewrite_imd_v2;
		bool opt_rewrite_imd_v3;
		bool opt_write_md5_files;
	};

	Ref<ImportExporterReport> report;
	void _do_export(uint32_t i, ExportToken *tokens);
	Error handle_auto_converted_file(const String &autoconverted_file, const String &output_dir);
	Error rewrite_import_source(const String &rel_dest_path, const String &output_dir, const Ref<ImportInfo> &iinfo);
	static Vector<String> get_v2_wildcards();
	String _get_path(const String &output_dir, const String &p_path);
	void report_unsupported_resource(const String &type, const String &format_name, const String &import_path);
	Error remove_remap_and_autoconverted(const String &src, const String &dst, const String &output_dir);
	void rewrite_metadata(ExportToken &token);
	Error unzip_and_copy_addon(const Ref<ImportInfoGDExt> &iinfo, const String &zip_path, const String &output_dir);

protected:
	static void _bind_methods();

public:
	String get_session_notes_string();
	String get_detected_unsupported_resource_string();
	Error recreate_plugin_config(const String &output_dir, const String &plugin_dir);
	Error recreate_plugin_configs(const String &output_dir, const Vector<String> &plugin_dirs = {});
	Error remap_resource(const String &output_dir, Ref<ImportInfo> &iinfo);
	Error convert_res_txt_2_bin(const String &output_dir, const String &p_path, const String &p_dst);
	Error convert_res_bin_2_txt(const String &output_dir, const String &p_path, const String &p_dst);
	Error convert_tex_to_png(const String &output_dir, const String &p_path, const String &p_dst);
	Error convert_sample_to_wav(const String &output_dir, const String &p_path, const String &p_dst);
	Error convert_oggstr_to_ogg(const String &output_dir, const String &p_path, const String &p_dst);
	Error convert_mp3str_to_mp3(const String &output_dir, const String &p_path, const String &p_dst);
	Error decompile_scripts(const String &output_dir, const Vector<String> &files = {});

	void set_multi_thread(bool p_enable);

	Error _export_imports(const String &output_dir, const Vector<String> &files_to_export, EditorProgressGDDC *pr, String &error_string);
	Error export_imports(const String &output_dir = "", const Vector<String> &files_to_export = {});
	Ref<ImportExporterReport> get_report();
	void reset_log();
	void reset();
	ImportExporter();
	~ImportExporter();
};

#endif
