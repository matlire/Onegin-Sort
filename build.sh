gcc -fsanitize=address,undefined -O1 -Wall -Wextra -Wno-unused-function -lm -I./src src/main.c src/sorting/sorting.c src/io/io.c -o dist/main.out && ./dist/main.out
