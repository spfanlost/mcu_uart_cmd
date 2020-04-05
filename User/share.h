
#ifndef _SHARE_H_
#define _SHARE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

typedef signed char char_t;
typedef signed short int short_t;
typedef signed int int_t;
typedef signed long int long_t;
typedef unsigned char byte_t;
typedef unsigned short int word_t;
typedef unsigned int dword_t;
typedef unsigned long int qword_t;

typedef int (*CaseFunc_t)(void);
typedef void (*VoidFunc_t)(void);
//--------------------------------------------------------------------------------

/* Test case description structure */
typedef struct _TestCase_t
{
    CaseFunc_t CaseFunc;  /* Case function                        */
    const char *CaseName; /* Case function name string            */
    dword_t En;           /* Case function enabled                */
    // VoidFunc_t PreFunc;
    // VoidFunc_t EndFunc;
    int_t Result;
} TestCase_t;

/* Test suite description structure */
typedef struct _TestSuite_t
{
    const char *FileName;       /* Test module file name                */
    const char *Date;           /* Compilation date                     */
    const char *Time;           /* Compilation time                     */
    const char *TestSuiteTitle; /* Title or name of module under test   */
    TestCase_t *TestCase;       /* Array of test cases                  */
    dword_t NumOfTestCase;      /* Number of test cases (sz of TC array)*/
} TestSuite_t;

/* Assertion statistics */
typedef struct _AssertStat_t
{
    dword_t passed;   /* Total assertions passed              */
    dword_t failed;   /* Total assertions failed              */
    dword_t warnings; /* Total assertions warnings            */
} AssertStat_t;

/* Test global statistics */
typedef struct _TestReport_t
{
    dword_t tests;           /* Total test cases count               */
    dword_t executed;        /* Total test cases executed            */
    dword_t passed;          /* Total test cases passed              */
    dword_t failed;          /* Total test cases failed              */
    dword_t warnings;        /* Total test cases warnings            */
    AssertStat_t assertions; /* Total assertions statistics          */
} TestReport_t;

dword_t test_list_exe(TestCase_t *TestList, dword_t NumOfCase);

enum ASCII
{
    NUL = 0,
    SOH,
    STX,
    ETX,
    EOT,
    ENQ,
    ACK,
    BEL,
    BS,
    HT,
    LF,
    VT,
    FF,
    CR,
    SO,
    SI,
    DLE,
    DC1,
    DC2,
    DC3,
    DC4,
    NAK,
    SYN,
    ETB,
    CAN,
    EM,
    SUB,
    ESC,
    FS,
    GS,
    RS,
    US,
    SPACE, //32
    DEL = 127
};

enum CMD_VAL
{
    CMD_INIT,
    CMD_VAL1,
    CMD_VAL2,
    CMD_VAL3,
    CMD_NUM
};

typedef struct
{
    void (*pfn)(char *str, byte_t *pos);
    char cmd[8];
    byte_t cmdlen;
    byte_t paranum;
    byte_t rev[2];
} uart_cmd_t;

/* Test case definition macro */
// #define DISABLE 0
// #define ENABLE 1
#define SUCCEED 1
#define FAILED 0
#define TRUE 1
#define FALSE 0

#ifndef NULL
 #ifdef __cplusplus              // EC++
  #define NULL          0
 #else
  #define NULL          ((void *) 0)
 #endif
#endif

// TestCaseDefine
#define TCD(tf, en) { tf, #tf, en, FAILED }
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#define MEM32_GET(n) (*(volatile dword_t *)(n))
#define MEM16_GET(n) (*(volatile word_t *)(n))
#define MEM8_GET(n) (*(volatile byte_t *)(n))
#define REG_GET(n) (*(volatile dword_t *)(n))

#define RAND_INIT() (srand(time(NULL)))
#define BYTE_RAND() ((byte_t)rand())
#define WORD_RAND() ((word_t)rand())
#define DWORD_RAND() ((dword_t)rand() << 16 | rand())
#define RAND_RANGE(s, e) (rand() % ((e) - (s) + 1) + (s))

//--------------------------------------------------------------------------------
#define LOG_DBUG 0x10000000
#define LOG_INFO 0x20000000
#define LOG_WARN 0x40000000
#define LOG_ERROR 0x80000000
#define ALL_LOG 0xFFFFFFFF
#define LOG_FILTER (LOG_INFO | LOG_WARN | LOG_ERROR) //|LOG_DBUG)
// #define LOG_FILTER (ALL_LOG)
#define LOG_PRINT_IF printf
#define LOG(filter, fmt, ...)                                                                                              \
    do                                                                                                                     \
    {                                                                                                                      \
        if (filter & LOG_FILTER)                                                                                           \
        {                                                                                                                  \
            if ((filter & LOG_ERROR) == LOG_ERROR)                                                                         \
            {                                                                                                              \
                LOG_PRINT_IF("\033[31mERROR!!! %s:%d %s()\033[0m: " fmt, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__); \
            }                                                                                                              \
            else if ((filter & LOG_WARN) == LOG_WARN)                                                                      \
            {                                                                                                              \
                LOG_PRINT_IF("\033[33mWARNING! %s:%d %s()\033[0m: " fmt, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__); \
            }                                                                                                              \
            else                                                                                                           \
                LOG_PRINT_IF(fmt, ##__VA_ARGS__);                                                                          \
        }                                                                                                                  \
    } while (0)
//--------------------------------------------------------------------------------
// JUST FOR DEBUG
#define BLACK_LOG "30"  //黑色
#define RED_LOG "31"    //红色
#define GREEN_LOG "32"  //绿色
#define YELLOW_LOG "33" //黄色
#define BLUE_LOG "34"   //蓝色
#define PURPLE_LOG "35" //紫色
#define SKBLU_LOG "36"  //天蓝
#define WHITE_LOG "37"  //白色

#define DISP_LOG_COLOR
#ifdef DISP_LOG_COLOR
#define LOG_COLOR(color, format, ...) LOG_PRINT_IF("\033[" color "m" format "\033[0m", ##__VA_ARGS__)
#define COLOR_STR(color, str) ("\033[" color "m" str "\033[0m")
#else
#define LOG_COLOR(color, format, ...) ((void)0)
#endif
//--------------------------------------------------------------------------------
//debug log comm start
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//debug log comm end
//--------------------------------------------------------------------------------
#define TEST_PASS "\n"                                \
                  "PPPPPP   AAAA   SSSSSS  SSSSSS \n" \
                  "PP  PP  AA  AA  SS      SS     \n" \
                  "PPPPPP  AAAAAA  SSSSSS  SSSSSS \n" \
                  "PP      AA  AA      SS      SS \n" \
                  "PP      AA  AA  SSSSSS  SSSSSS \n\n"
#define TEST_FAIL "\n"                                \
                  "FFFFFF   AAAA   IIIIII  LL     \n" \
                  "FF      AA  AA    II    LL     \n" \
                  "FFFFFF  AAAAAA    II    LL     \n" \
                  "FF      AA  AA    II    LL     \n" \
                  "FF      AA  AA  IIIIII  LLLLLL \n\n"

//*********************


void uart_cmd(void);

#endif
