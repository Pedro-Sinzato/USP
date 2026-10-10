#ifndef NASA_H
#define NASA_H

/*!
 * \brief Estrutura para representar um ASTEROIDE.
 *  Guarda os campos de ASTEROIDE.
 */
typedef struct asteroid ASTEROID;

/**
 * @brief Cria ASTEROID
 * @return Ponteiro para ASTEROID, se não conseguir, NULL.
 */
ASTEROID *create_asteroid(void);

/**
 * @brief Lê a DATABASE.
 * @param[in] Nome do arquivo DATABASE.
 * @return TRUE se conseguir e FALSE se não conseguir.
 */
extern bool read_database(char database_name[]);

#endif
