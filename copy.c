#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

int main(int argc, char *argv[]){
    if (argc != 3){
        printf("Формат записи: copy <исходный файл> <целевой файл>\n");
        exit(1);
    }

    int fd_orig = open(argv[1], O_RDONLY);
    if (fd_orig == -1){
        fprintf(stderr, "Ошибка открытия исходного файла: %s\n", strerror(errno));
        exit(1);
    }
    
    int fd_copy = open(argv[2], O_CREAT|O_WRONLY|O_TRUNC, 0644);
    if (fd_copy == -1){
        fprintf(stderr, "Ошибка открытия целевого файла: %s\n", strerror(errno));
        close(fd_orig);
        exit(1);
    }
    
    char buffer[BUFSIZ];
    ssize_t bytes_read, bytes_write;

    while((bytes_read = read(fd_orig, buffer, BUFSIZ)) > 0){
        bytes_write = write(fd_copy, buffer, bytes_read);
        if (bytes_read != bytes_write){
            perror("ошибка записи");
            close(fd_orig);
            close(fd_copy);
            exit(1);
        }
    }

    if (bytes_read == -1){
        fprintf(stderr, "Ошибка чтения: %s\n", strerror(errno));
        close(fd_orig);
        close(fd_copy);
        exit(1);
    }

    if (bytes_write == -1){
        fprintf(stderr, "Ошибка записи: %s\n", strerror(errno));
        close(fd_orig);
        close(fd_copy);
        exit(1);
    }

    printf("PID процесса: %d\n", getpid());
    printf("Программа скопировала файл. Нажмите Enter для завершения...\n");
    getchar();

    close(fd_orig);
    close(fd_copy);
    
    return 0;
}
