; ============================================================
; MACRO : mAABB_CHECK
; Rôle  : Teste le chevauchement de deux rectangles AABB.
;         Génère du code INLINE (pas de CALL).
; Entrée : Variables globales AABB1_X, AABB1_W, AABB1_Y, AABB1_H
;          et AABB2_X, AABB2_W, AABB2_Y, AABB2_H (contiguës).
; Sortie : Carry = 1 si chevauchement strict, Carry = 0 sinon.
; Détruit : AF, B, HL
; Note   : Le contact bord à bord n'est PAS considéré comme
;          une collision (contact exclu : si X2 = X1 + W1 → Carry = 0).
; ============================================================
MACRO mAABB_CHECK

    ; ---- TEST X (1) : X2 < X1 + W1 ? ----
    LD   HL, AABB1_X
    LD   A, (HL)              ; A = X1
    INC  HL
    ADD  A, (HL)              ; A = X1 + W1 = fin1_x
    LD   B, A                 ; B = fin1_x

    LD   HL, AABB2_X
    LD   A, (HL)              ; A = X2
    CP   B                    ; Compare X2 avec fin1_x
    JR   NC, @no_collision    ; Si X2 >= fin1_x → séparés (contact exclu)

    ; ---- TEST X (2) : X1 < X2 + W2 ? ----
    LD   HL, AABB2_X
    LD   A, (HL)              ; A = X2
    INC  HL
    ADD  A, (HL)              ; A = X2 + W2 = fin2_x
    LD   B, A                 ; B = fin2_x

    LD   HL, AABB1_X
    LD   A, (HL)              ; A = X1
    CP   B                    ; Compare X1 avec fin2_x
    JR   NC, @no_collision    ; Si X1 >= fin2_x → séparés (contact exclu)

    ; ---- TEST Y (1) : Y2 < Y1 + H1 ? ----
    LD   HL, AABB1_Y
    LD   A, (HL)              ; A = Y1
    INC  HL
    ADD  A, (HL)              ; A = Y1 + H1 = fin1_y
    LD   B, A                 ; B = fin1_y

    LD   HL, AABB2_Y
    LD   A, (HL)              ; A = Y2
    CP   B                    ; Compare Y2 avec fin1_y
    JR   NC, @no_collision    ; Si Y2 >= fin1_y → séparés (contact exclu)

    ; ---- TEST Y (2) : Y1 < Y2 + H2 ? ----
    LD   HL, AABB2_Y
    LD   A, (HL)              ; A = Y2
    INC  HL
    ADD  A, (HL)              ; A = Y2 + H2 = fin2_y
    LD   B, A                 ; B = fin2_y

    LD   HL, AABB1_Y
    LD   A, (HL)              ; A = Y1
    CP   B                    ; Compare Y1 avec fin2_y
    JR   NC, @no_collision    ; Si Y1 >= fin2_y → séparés (contact exclu)

    ; ---- CHEVAUCHEMENT STRICT CONFIRMÉ ----
    SCF                       ; Carry = 1
    JR   @end_check

@no_collision:
    OR   A                    ; Carry = 0

@end_check:
    ; Les flags sont prêts (Carry = 1 si chevauchement strict, 0 sinon)
ENDM