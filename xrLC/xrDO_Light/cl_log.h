#pragma	once

void __cdecl	clMsg		(const char *format, ...);
void __cdecl	Status		(const char *format, ...);
void Progress	(const float F);
void Phase		(const char *phase_name);

void			logThread	(void *dummy);
void __stdcall	logCallback	(LPCSTR c);
