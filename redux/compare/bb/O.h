#include  "local.h"
#define T_Object        1
#define V_Object        2
#define R_Object        3
#define U_Object        4
#define A_Object        5
typedef unsigned short SHORT ;
#define MAXSHORT        0177777
typedef struct {
  SHORT R_a ;
  SHORT R_b ;
}
R_row ;
typedef struct {
  SHORT A_a ;
  SHORT A_b ;
  SHORT A_c ;
}
A_row ;
typedef struct T_desc {
  int Type ;
  int T_n ;
  int T_lname ;
  int T_lhash ;
  char ** T_name ;
  SHORT * T_hash ;
}
* T_OBJECT ;
typedef struct V_desc {
  int Type ;
  int V_n ;
  int V_lvec ;
  int V_lhash ;
  SHORT ** V_vec ;
  SHORT * V_hash ;
}
* V_OBJECT ;
typedef struct R_desc {
  int Type ;
  int R_n ;
  int R_lrec ;
  int R_lhash ;
  R_row * R_rec ;
  SHORT * R_hash ;
}
* R_OBJECT ;
typedef struct U_desc {
  int Type ;
  int U_n ;
  int U_lrec ;
  int U_lhash ;
  A_row * U_rec ;
  SHORT * U_hash ;
}
* U_OBJECT ;
typedef struct A_desc {
  int Type ;
  SHORT A_mode ;
  SHORT A_ems ;
  int A_nT ;
  int A_nQ ;
  int A_nS ;
  int A_nrows ;
  int A_lrows ;
  A_row ** A_p ;
  A_row * A_t ;
}
* A_OBJECT ;
#define OPEN            0
#define NFA             1
#define NFA_TRIM        2
#define NFA_EQLAM       3
#define NFA_CMLAM       4
#define NFA_CLOSED      5
#define DFA             6
#define DFA_MIN         7
#define SSEQ            8
#define SSEQ_MIN        9
#define START           0
#define FINAL           1
extern int A_report ;
A_OBJECT A_create ( ) ;
void A_destroy ( A_OBJECT ) ;
A_OBJECT A_rept ( A_OBJECT ) ;
void A_exchange ( A_OBJECT, A_OBJECT ) ;
A_OBJECT A_copy ( A_OBJECT ) ;
A_OBJECT A_deems ( A_OBJECT ) ;
A_OBJECT A_adems ( A_OBJECT ) ;
A_OBJECT A_add ( A_OBJECT, int, int, int ) ;
A_OBJECT A_open ( A_OBJECT ) ;
A_OBJECT A_close ( A_OBJECT ) ;
A_OBJECT A_rename ( A_OBJECT, SHORT * ) ;
A_OBJECT A_mkdense ( A_OBJECT ) ;
char * get_name ( ) ;
void put_name ( char * ) ;
int get_nl ( ) ;
A_OBJECT A_load ( char *, T_OBJECT ) ;
A_OBJECT A_store ( A_OBJECT, char *, T_OBJECT ) ;
A_OBJECT A_save ( A_OBJECT, char *, T_OBJECT ) ;
A_OBJECT A_lwds ( char *, T_OBJECT ) ;
A_OBJECT A_prsseq ( A_OBJECT, char *, T_OBJECT ) ;
SHORT A_tr_DFS ( SHORT ) ;
A_OBJECT A_trim ( A_OBJECT ) ;
int A_la_DFS ( int ) ;
A_OBJECT A_lameq ( A_OBJECT ) ;
int A_cm_DFS ( int ) ;
A_OBJECT A_lamcm ( A_OBJECT ) ;
void A_cl_DFS ( int ) ;
A_OBJECT A_clsure ( A_OBJECT ) ;
A_OBJECT A_subs ( A_OBJECT ) ;
A_OBJECT A_min ( A_OBJECT ) ;
void A_conform ( A_OBJECT, A_OBJECT ) ;
A_OBJECT A_phi ( ) ;
A_OBJECT A_lambda ( ) ;
A_OBJECT A_letter ( int, int ) ;
A_OBJECT A_deecho ( A_OBJECT, int, int ) ;
A_OBJECT A_opt ( A_OBJECT ) ;
A_OBJECT A_plus ( A_OBJECT ) ;
A_OBJECT A_star ( A_OBJECT ) ;
A_OBJECT A_union ( A_OBJECT, A_OBJECT ) ;
A_OBJECT A_percent ( A_OBJECT, A_OBJECT ) ;
A_OBJECT A_concat ( A_OBJECT, A_OBJECT ) ;
A_OBJECT A_intersect ( A_OBJECT, A_OBJECT ) ;
A_OBJECT A_differ ( A_OBJECT, A_OBJECT ) ;
A_OBJECT A_xor ( A_OBJECT, A_OBJECT ) ;
A_OBJECT A_alph ( A_OBJECT ) ;
A_OBJECT A_rev ( A_OBJECT ) ;
A_OBJECT A_shuffle ( A_OBJECT, A_OBJECT ) ;
int A_en_DFS ( SHORT ) ;
A_OBJECT A_enum ( A_OBJECT, T_OBJECT, int ) ;
int A_cd_DFS ( SHORT ) ;
int A_card ( A_OBJECT ) ;
A_OBJECT A_pref ( A_OBJECT ) ;
A_OBJECT A_suff ( A_OBJECT ) ;
A_OBJECT A_compose ( A_OBJECT, A_OBJECT ) ;
A_OBJECT A_join ( A_OBJECT, A_OBJECT ) ;
A_OBJECT A_retape ( A_OBJECT, A_OBJECT, T_OBJECT ) ;
A_OBJECT A_comma ( A_OBJECT, A_OBJECT ) ;
A_OBJECT A_catpow ( A_OBJECT, int ) ;
A_OBJECT A_ident ( A_OBJECT ) ;
A_OBJECT A_cmpow ( A_OBJECT, int ) ;
A_OBJECT A_lenmin ( A_OBJECT ) ;
int A_minlen ( A_OBJECT ) ;
A_OBJECT A_sseq ( A_OBJECT ) ;
int GMcmp ( SHORT *, SHORT * ) ;
A_OBJECT A_GMsseq ( A_OBJECT ) ;
A_OBJECT A_LMsseq ( A_OBJECT ) ;
A_OBJECT A_clsseq ( A_OBJECT ) ;
void A_st_free ( ) ;
int A_st_DFS ( int, int ) ;
SHORT ** A_stems ( A_OBJECT, int ) ;
void A_prstems ( A_OBJECT, T_OBJECT, int ) ;
T_OBJECT T_create ( ) ;
void T_destroy ( T_OBJECT ) ;
int T_member ( T_OBJECT, char * ) ;
T_OBJECT T_grow ( T_OBJECT, int ) ;
int T_insert ( T_OBJECT, char * ) ;
char * T_name ( T_OBJECT, int ) ;
void T_stats ( ) ;
SHORT * veccpy ( SHORT *, SHORT * ) ;
int veccmp ( SHORT *, SHORT * ) ;
int veclen ( SHORT * ) ;
V_OBJECT V_create ( ) ;
void V_destroy ( V_OBJECT ) ;
int V_member ( V_OBJECT, SHORT * ) ;
V_OBJECT V_grow ( V_OBJECT, int ) ;
int V_insert ( V_OBJECT, SHORT * ) ;
SHORT * V_vec ( V_OBJECT, int ) ;
void V_stats ( ) ;
R_OBJECT R_create ( ) ;
void R_destroy ( R_OBJECT ) ;
int R_member ( R_OBJECT, int, int ) ;
R_OBJECT R_grow ( R_OBJECT, int ) ;
int R_insert ( R_OBJECT, int, int ) ;
R_row * R_rec ( R_OBJECT, int ) ;
void R_stats ( ) ;
U_OBJECT U_create ( ) ;
void U_destroy ( U_OBJECT ) ;
int U_member ( U_OBJECT, int, int, int ) ;
U_OBJECT U_grow ( U_OBJECT, int ) ;
int U_insert ( U_OBJECT, int, int, int ) ;
A_row * U_rec ( U_OBJECT, int ) ;
void U_stats ( ) ;
void copymem ( long, char *, char * ) ;
char * Salloc ( long ) ;
void Sfree ( char * ) ;
char * Srealloc ( char *, long ) ;
char * Scopy ( char * ) ;
long Ssize ( char * ) ;
char * Sfind ( char * ) ;
void Sarena ( ) ;
void Saudit ( ) ;
void Warning ( char * ) ;
void Error ( char * ) ;
SHORT * s_alloc ( int ) ;
int * i_alloc ( int ) ;
void pr_time_diff ( ) ;
char * pad20 ( char * ) ;
char * copyof ( char * ) ;
int yylex ( ) ;
void yyerror ( char * ) ;
int tonum ( char * ) ;
int do_n_i ( char * ) ;
A_OBJECT do_an_a ( A_OBJECT, char * ) ;
A_OBJECT do_ann_a ( A_OBJECT, char *, char * ) ;
A_OBJECT do_nn_a ( char *, char * ) ;
int yyparse ( void ) ;
