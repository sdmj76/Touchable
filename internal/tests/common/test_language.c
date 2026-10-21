/* Build with -Iinternal/runtime/common. Checks the app-language to game-language mapping. */
#include "ss_language.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static void check(const char *tag, const char *expected) {
    const char *actual = ss_switch_language(tag);
    if (strcmp(actual, expected)) {
        fprintf(stderr, "tag %s: expected %s, got %s\n", tag ? tag : "(null)", expected, actual);
        assert(0);
    }
}

int main(void) {
    /* The script subtag wins over the region. Hong Kong uses zh-Hans-HK and zh-Hant-HK. */
    check("zh-Hans-HK", "zh-Hans");
    check("zh-Hant-HK", "zh-Hant");
    check("zh-Hans-TW", "zh-Hans");
    check("zh-Hant-CN", "zh-Hant");
    check("zh-Hans", "zh-Hans");
    check("zh-Hant", "zh-Hant");
    check("cmn-Hans-HK", "zh-Hans");
    /* No script subtag: the region selects the written form. */
    check("zh-HK", "zh-Hant");
    check("zh-TW", "zh-Hant");
    check("zh-MO", "zh-Hant");
    check("zh-CN", "zh-Hans");
    check("zh-SG", "zh-Hans");
    check("zh", "zh-Hans");
    check("zh_TW", "zh-Hant");
    check("yue-Hant-HK", "zh-Hant");
    check("yue-Hans-HK", "zh-Hans");   /* the script subtag also wins for Cantonese */
    check("yue", "zh-Hant");
    check("yue-CN", "zh-Hant");
    /* Other languages keep the existing mapping. */
    check("en-US", "en-US");
    check("en-HK", "en-US");
    check("en-GB", "en-GB");
    check("en-AU", "en-GB");
    check("fr", "fr");
    check("fr-CA", "fr-CA");
    check("es-ES", "es");
    check("es-MX", "es-419");
    check("ja-JP", "ja");
    check("ko-KR", "ko");
    check("de-DE", "de");
    check("it-IT", "it");
    check("nl-NL", "nl");
    check("th-TH", "en-US");
    check(NULL, "zh-Hans");
    check("", "zh-Hans");
    puts("test_language: ok");
    return 0;
}
