// 🪄 Layer.h — Manejo de capas, scroll y fusión inteligente
// Compatible con Canvas.h y Graphic.h

#ifndef _LAYER_H_
#define _LAYER_H_

#include "Canvas.h"

#ifndef TRANSPARENT
#define TRANSPARENT 0x00
#endif

// ================================================================
// 🧱 Stack de capas
// ================================================================
typedef struct {
    int count;
    Canvas **list;
} CanvasLayerStack;

static inline CanvasLayerStack * CanvasLayer_Create(int count, int width, int height)
{
    CanvasLayerStack *stack = allocate(CanvasLayerStack);
    if (!stack) return NULL;
    stack->count = count;
    stack->list = allocateArray(Canvas*, count);
    for (int i = 0; i < count; i++)
        stack->list[i] = Canvas_Create(width, height);
    return stack;
}

static inline void CanvasLayer_Destroy(CanvasLayerStack *stack)
{
    if (!stack) return;
    for (int i = 0; i < stack->count; i++)
        Canvas_Destroy(stack->list[i]);
    free(stack->list);
    free(stack);
}

// ================================================================
// 🧩 Canvas_MergerLayer — fusión de capas
// ================================================================
//static inline void Canvas_MergeLayer(Canvas *dest, Canvas *src, int dstX, int dstY)
static inline void Canvas_MergeLayer(Canvas *dest, Canvas *src)
{
    if (!dest || !src) return;
    
    for (int sy = 0; sy < src->height; sy++)
    {
        int dy =  sy;
        if (dy < 0 || dy >= dest->height) continue;

        int srcLogicalY = sy / 2;
        int destLogicalY = dy / 2;
        if (srcLogicalY >= src->logicalHeight || destLogicalY >= dest->logicalHeight) continue;

        int destIsUpper = (dy % 2 == 0);

        for (int sx = 0; sx < src->width; sx++)
        {
            int dx = sx;
            if (dx < 0 || dx >= dest->width) continue;

            BYTE srcColor = src->buffer[srcLogicalY * src->width + sx].color;
            if (srcColor == TRANSPARENT) continue;

            BYTE fg = (srcColor >> 4) & 0x0F;
            BYTE bg = srcColor & 0x0F;
            Pixel *dp = &dest->buffer[destLogicalY * dest->width + dx];

            if (destIsUpper)
            {
                if (fg != 0) dp->color = (fg << 4) | (dp->color & 0x0F);
            }
            else
            {
                if (bg != 0) dp->color = (dp->color & 0xF0) | bg;
            }
        }
    }
}

// ================================================================
// 🌊 Canvas_BlitSmooth — desplazamiento de medio carácter
// ================================================================
static inline void Canvas_BlitSmooth(Canvas *dest, Canvas *src, int dstX, int dstY)
{
    int halfShift = dstY % 2; // 0 = normal, 1 = desplazado medio carácter

    for (int sy = 0; sy < src->height; sy++)
    {
        int dy = dstY + sy;
        if (dy < 0 || dy >= dest->height) continue;

        int srcLogicalY = sy / 2;
        if (srcLogicalY >= src->logicalHeight) continue;

        int destLogicalY = dy / 2;
        int nextDestLogicalY = destLogicalY + (halfShift && (dy % 2 == 1) ? 1 : 0);
        if (destLogicalY >= dest->logicalHeight) continue;

        int srcIsUpper = (sy % 2 == 0);
        int destIsUpper = (dy % 2 == 0);

        for (int sx = 0; sx < src->width; sx++)
        {
            int dx = dstX + sx;
            if (dx < 0 || dx >= dest->width) continue;

            BYTE srcColor = src->buffer[srcLogicalY * src->width + sx].color;
            if (srcColor == TRANSPARENT) continue;

            BYTE fg = (srcColor >> 4) & 0x0F;
            BYTE bg = srcColor & 0x0F;

            if (!halfShift)
            {
                // --- Modo normal ---
                Pixel *dp = &dest->buffer[destLogicalY * dest->width + dx];
                if (destIsUpper)
                {
                    if (fg != 0) dp->color = (fg << 4) | (dp->color & 0x0F);
                }
                else
                {
                    if (bg != 0) dp->color = (dp->color & 0xF0) | bg;
                }
            }
            else
            {
                // --- Modo desplazado medio carácter ---
                // Mezclar el píxel superior del src con el píxel inferior del dest
                Pixel *dp1 = &dest->buffer[destLogicalY * dest->width + dx];
                Pixel *dp2 = (nextDestLogicalY < dest->logicalHeight)
                                 ? &dest->buffer[nextDestLogicalY * dest->width + dx]
                                 : NULL;

                if (srcIsUpper)
                {
                    // superior del src → inferior del dest
                    if (fg != 0)
                        dp1->color = (dp1->color & 0xF0) | fg;
                }
                else
                {
                    // inferior del src → superior de la siguiente fila lógica
                    if (dp2 && bg != 0)
                        dp2->color = (bg << 4) | (dp2->color & 0x0F);
                }
            }
        }
    }
}

// ================================================================
// 🌀 Canvas_ScrollY — desplazamiento vertical (con wrap opcional)
// ================================================================
static inline void Canvas_ScrollY(Canvas *c, int offset, int wrap)
{
    if (!c || offset == 0) return;
    int len = c->width * c->logicalHeight;
    Pixel *temp = allocateArray(Pixel, len);
    memset(temp, 0, len * sizeof(Pixel));

    for (int y = 0; y < c->height; y++)
    {
        int ny = y + offset;
        if (wrap)
        {
            ny = (ny + c->height) % c->height;
        }
        if (ny < 0 || ny >= c->height) continue;

        int srcLogicalY = y / 2;
        int destLogicalY = ny / 2;
        if (srcLogicalY >= c->logicalHeight || destLogicalY >= c->logicalHeight) continue;

        temp[destLogicalY * c->width + 0] = c->buffer[srcLogicalY * c->width + 0];
        memcpy(&temp[destLogicalY * c->width],
               &c->buffer[srcLogicalY * c->width],
               c->width * sizeof(Pixel));
    }

    memcpy(c->buffer, temp, len * sizeof(Pixel));
    free(temp);
}

// ================================================================
// 🌀 Canvas_ScrollX — desplazamiento horizontal
// ================================================================
static inline void Canvas_ScrollX(Canvas *c, int offset, int wrap)
{
    if (!c || offset == 0) return;

    for (int y = 0; y < c->logicalHeight; y++)
    {
        Pixel *row = allocateArray(Pixel, c->width);
        memcpy(row, &c->buffer[y * c->width], c->width * sizeof(Pixel));

        for (int x = 0; x < c->width; x++)
        {
            int nx = x + offset;
            if (wrap)
            {
                nx = (nx + c->width) % c->width;
            }
            if (nx < 0 || nx >= c->width) continue;
            c->buffer[y * c->width + nx] = row[x];
        }
        free(row);
    }
}

// ================================================================
// 🪄 CanvasLayer_MergeAll — combina todas las capas en una
// ================================================================
static inline Canvas * CanvasLayer_MergeAll( CanvasLayerStack *stack)
{
    Canvas *dest;
    if (!stack) return NULL;
    dest = Canvas_Create(stack->list[0]->width, stack->list[0]->height);
    for (int i = 0; i < stack->count; i++)
        Canvas_MergeLayer(dest, stack->list[i]);
    return dest;
}

typedef struct
{
    CanvasLayerStack* (*Create)(int,int,int);
    void (*Destroy)(CanvasLayerStack *);
    void (*MergeLayer)(Canvas *, Canvas *);
    void (*MergeCanvas)(Canvas *, Canvas *,int, int);
    void (*ScrollX)(Canvas *, int, int);
    void (*ScrollY)(Canvas *, int, int);
    Canvas * (*Resume)(CanvasLayerStack*);
} Layer;

Layer* Layer_Init(void)
{
    Layer *g = allocate(Layer);
    g->Create = CanvasLayer_Create;
    g->Destroy = CanvasLayer_Destroy;
    g->MergeCanvas = Canvas_BlitSmooth;
    g->MergeLayer = Canvas_MergeLayer;
    g->ScrollX = Canvas_ScrollX;
    g->ScrollY = Canvas_ScrollX;
    g->Resume = CanvasLayer_MergeAll;
    return g;
}
#endif // _LAYER_H_
