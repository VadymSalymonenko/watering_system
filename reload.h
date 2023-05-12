
byte days_to_reload = 2;
byte current_day = 8;

boolean is_time_to_reload();
void reload();
boolean is_day_changed(byte clock_dow);
void reload_recovery();
