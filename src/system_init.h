#ifndef SYSTEM_INIT_H
#define SYSTEM_INIT_H

/**
 * @brief Initialize base system requirements. This should
 * be the first call to be invoked, before any other modules
 * are loaded by the application.
 */
void system_pre_init();

/**
 * @brief Finalize system initialization. This should be the
 * last call to be invoked, after all other modules are loaded
 * by the application.
 */
void system_post_init();

#endif
