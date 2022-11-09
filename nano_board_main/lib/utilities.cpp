


#define PF_BUFFER_SIZE 255
#define pf(fmt, ...) _pf(PSTR(fmt), __VA_ARGS__)

char pf_buffer[PF_BUFFER_SIZE];
void _pf(const char *fmt, ...) {
    va_list valist;
    va_start(valist, fmt);
    vsnprintf(pf_buffer, sizeof(pf_buffer), fmt, valist);
    va_end(valist);
    Serial.println(pf_buffer);
}

