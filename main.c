#include <stdio.h>
#include "chese.h"
#pragma warning(disable:4996)

int main() {
    board *pboard = create();
    canvas *pcanvas = create_canvas();
    draw_board(pboard, pcanvas);
    printf("%s", pcanvas);
    int src, dsc;
    for(;;)
    {
        scanf("%d %d", &src, &dsc);
        if(src == -1) break;
        int code = move(src, dsc, pboard);
        printf("%d\n", code);
        draw_board(pboard, pcanvas);
        printf("%s", pcanvas);
    }
    destory(pboard);
    destory_canvas(pcanvas);
    return 0;
}
