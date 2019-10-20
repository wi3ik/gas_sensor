#ifndef __WEB_HANDLER_H__
#define __WEB_HANDLER_H__

int init_web_handlers();

void configureAccessPointMode();
void configureServerMode();

void configure_mDNS();

void webServiceHandler(void);

void handleNotFound();


bool is_ap_mode();
void toggle_led();
void get_led_state();


#endif /* __WEB_HANDLER_H__ */
