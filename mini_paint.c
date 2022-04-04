#include <unistd.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int W;
int H;
char BG;
char **TAB;

typedef struct shape
{
    char t;
    float x;
    float y;
    float r;
    char c;
} shape;

int msg_error(FILE *fd, int error)
{
    if (error == 2)
    {
        write(1, "Error: Operation file corrupted\n", 32);
        if (fd)
            fclose(fd);
        return (1);
    }
    else if (error == 1)
        write(1, "Error: argument\n", 16);
    else
    {
        for (int i = 0; i < H; i++)
        {
            write(1, TAB[i], W);
            write(1, "\n", 1);
        }
    }
    if (fd)
        fclose(fd);
    return (error);
}

int main(int argc, char **argv)
{
    FILE *fd;
    shape rnd;
    int res;
    float sqr;
    fd = NULL;

    if (argc != 2)
        return (msg_error(fd, 1));
    if ((fd = fopen(argv[1], "r")))
    {
        if ((res = fscanf(fd, "%d %d %c", &W, &H, &BG)) == 3)
        {
            if (W <= 300 && W > 0 && H <= 300 && H > 0)
            {
                TAB = malloc(H * sizeof(char *));
                for (int i = 0; i < H; i++)
                {
                    TAB[i] = malloc(W * sizeof(char));
                    memset(TAB[i], BG, W);
                }
                while (1)
                {
                    res = fscanf(fd, "\n%c %f %f %f %c", &rnd.t, &rnd.x, &rnd.y, &rnd.r, &rnd.c);
                    if (res == -1)
                        return (msg_error(fd, 0));
                    else if (res != 5 || rnd.r <= 0 || (rnd.t != 'c' && rnd.t != 'C'))
                        break;
                    for (int line = 0; line < H; line++)
                    {
                        for (int col = 0; col < W; col++)
                        {
                            sqr = sqrtf((col - rnd.x) * (col - rnd.x) + (line - rnd.y) * (line - rnd.y));
                            if (sqr <= rnd.r)
                            {
                                if (rnd.t == 'c' && sqr + 1 > rnd.r)
                                    TAB[line][col] = rnd.c;
                                else if (rnd.t == 'C')
                                    TAB[line][col] = rnd.c;
                            }
                        }
                    }
                }
            }
        }
    }
    return (msg_error(fd, 2));
} 