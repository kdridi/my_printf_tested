#include "my.h"

#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>
#include <stdarg.h>

#include <uuid/uuid.h>

/*/
OS
+---+----------------------------------------------------+
| 0 | Informations pour lire depuis l'entrée standard    |
+---+----------------------------------------------------+
| 1 | Informations pour écrire vers le fichier out.log   |
+---+----------------------------------------------------+
| 2 | Informations pour écrire vers le fichier err.log   |
+---+----------------------------------------------------+
| 3 | Informations pour écrire vers le fichier out.log   |
+---+----------------------------------------------------+
| 4 | Informations pour écrire vers la sortie standard   |
+---+----------------------------------------------------+
| 5 | Informations pour écrire vers le fichier err.log   |
+---+----------------------------------------------------+
| 6 | Informations pour écrire vers la sortie erreur     |
+---+----------------------------------------------------+
/*/

char *file_out, *file_err;
int save_out, save_err;

static int watch_start_fd(int fd, char **filepath)
{
    uuid_t bid;
    uuid_generate_random(bid);

    char uuid[37];
    uuid_unparse_lower(bid, uuid);

    char filebuf[256];
    sprintf(filebuf, "criterion.%s.log", uuid);

    *filepath = strdup(filebuf);
    assert(*filepath != NULL);

    int new_fd = open(*filepath, O_WRONLY | O_CREAT | O_TRUNC, 0600);
    assert(new_fd != -1);

    int save_fd = dup(fd);
    assert(save_fd != -1);

    int status = dup2(new_fd, fd);
    assert(status != -1);

    status = close(new_fd);
    assert(status != -1);

    return (save_fd);
}

static char *watch_stop_fd(int fd, char *filepath, int save_fd)
{
    int file_fd;
    char *content;
    struct stat s;
    int status;

    status = stat(filepath, &s);
    assert(status != -1);

    content = malloc(s.st_size + 1);
    assert(content != NULL);
    content[s.st_size] = '\0';
    
    file_fd = open(filepath, O_RDONLY);
    assert(file_fd != -1);

    ssize_t read_size = read(file_fd, content, s.st_size);
    assert(read_size >= 0);
    assert(read_size == s.st_size);

    status = close(file_fd);
    assert(status != -1);

    status = dup2(save_fd, fd);
    assert(status != -1);

    status = close(save_fd);
    assert(status != -1);

    status = remove(filepath);
    assert(status != -1);

    free(filepath);
    
    return (content);
}

static void watch_start(void)
{
    save_out = watch_start_fd(STDOUT_FILENO, &file_out);
    save_err = watch_start_fd(STDERR_FILENO, &file_err);
}

static void watch_stop(char **out, char **err)
{
    *out = watch_stop_fd(STDOUT_FILENO, file_out, save_out);
    *err = watch_stop_fd(STDERR_FILENO, file_err, save_err);
}

static void cr_assert_printf(const char *message, char *fmt, ...)
{
    char *out, *err;
    va_list ap;

    watch_start();
    va_start(ap, fmt);
    int len = my_vprintf(fmt, ap);
    va_end(ap);
    watch_stop(&out, &err);
    
    cr_assert_str_eq(out, message);
    cr_assert_str_eq(err, "");
    cr_assert_eq((int) strlen(message), len);

    free(err);
    free(out);
}

Test(my_printf, exists)
{
    my_printf("Hello, world!");
}

Test(my_printf, simple_test)
{
    cr_assert_printf("Hello, world!", "Hello, world!");
}

Test(my_printf, simple_test_with_flag_s)
{
    cr_assert_printf("Hello, world!", "Hello, %s!", "world");
}

Test(my_printf, simple_test_with_flag_c)
{
    cr_assert_printf("Hello, Z!", "Hello, %c!", 'Z');
}

Test(my_printf, simple_test_with_flag_d)
{
    cr_assert_printf("Hello, -123!", "Hello, %d!", -123);
}