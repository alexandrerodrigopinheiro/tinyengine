#include "TinyEngine/OpenGL/FTGLFontManager.h"

#undef __FTERRORS_H__
#define FT_ERRORDEF(e, v, s)	{ (e), (s) },
#define FT_ERROR_START_LIST
#define FT_ERROR_END_LIST		{ 0, NULL }

struct ft_error {
	int err;
	const char * const str;
};

const struct ft_error ft_errors[] = {
#include FT_ERRORS_H
};

const char *ft_errorstring(int err) {
	const struct ft_error *e;

	for (e = ft_errors; e->str != NULL; e++) {
		if (e->err == err)
			return e->str;
	}

	return "Unknown error";
}

FTGLFontManager& FTGLFontManager::instance() {
	static FTGLFontManager tm;
	return tm;
}

FTGLFontManager::FTGLFontManager() {
	memset(_last_error, 0, sizeof(_last_error));
}

FTGLFontManager::~FTGLFontManager() {
	cleanup();
}

void FTGLFontManager::cleanup() {
	for (auto font = _fonts.begin(); font != _fonts.end(); font++) {
		delete (*font).second;
	}
	_fonts.clear();
}

FTFont *FTGLFontManager::getFont(const char *filename, int size) {
	char buf[PATH_MAX];
	snprintf(buf, sizeof(buf), "%s%i", filename, size);
	std::string fontKey = std::string(buf);

	auto result = _fonts.find(fontKey);
	if (result != _fonts.end()) {
		return result->second;
	}

	FTFont *font = new FTTextureFont(filename);
	if (font == NULL) {
		snprintf(_last_error, sizeof(_last_error), "Couldn't open %s", filename);
		return NULL;
	}

	if (font->Error() != FT_Err_Ok) {
		snprintf(_last_error, sizeof(_last_error), "%s", ft_errorstring(font->Error()));
		delete font;
		return NULL;
	}

	if (!font->FaceSize(size)) {
		snprintf(_last_error, sizeof(_last_error), "Failed to set size %i\n", size);
		delete font;
		return NULL;
	}

	_fonts[fontKey] = font;

	return font;
}

const char *FTGLFontManager::last_error() const {
	return _last_error;
}
