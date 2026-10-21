#pragma once
#include <string.h>
#include <strings.h>

/* Map an app language tag to a Nintendo language code. Unsupported tags fall back to US English. */
static inline const char *ss_switch_language(const char *tag) {
    if (!tag || !*tag) return "zh-Hans";
    char text[32];
    size_t length = strlen(tag);
    if (length >= sizeof text) length = sizeof text - 1;
    memcpy(text, tag, length); text[length] = 0;
    for (char *p = text; *p; ++p) if (*p == '_') *p = '-';
    char *parts[3] = {0};
    int count = 0;
    char *p = text;
    while (p && *p && count < 3) {
        parts[count++] = p;
        char *dash = strchr(p, '-');
        if (!dash) break;
        *dash = 0;
        p = dash + 1;
    }
    const char *language = parts[0];
    int cantonese = !strcasecmp(language, "yue");
    if (cantonese || !strcasecmp(language, "zh") || !strcasecmp(language, "cmn") ||
        !strcasecmp(language, "wuu") || !strcasecmp(language, "hak") ||
        !strcasecmp(language, "nan")) {
        /* The script subtag selects the written form. A region alone is only a fallback.
           The system list uses tags such as zh-Hans-HK and zh-Hant-HK in Hong Kong. */
        for (int i = 1; i < count; ++i)
            if (!strcasecmp(parts[i], "hans")) return "zh-Hans";
        for (int i = 1; i < count; ++i)
            if (!strcasecmp(parts[i], "hant")) return "zh-Hant";
        for (int i = 1; i < count; ++i)
            if (!strcasecmp(parts[i], "tw") || !strcasecmp(parts[i], "hk") ||
                !strcasecmp(parts[i], "mo")) return "zh-Hant";
        /* Cantonese without a script subtag is usually written in Traditional Chinese. */
        return cantonese ? "zh-Hant" : "zh-Hans";
    }
    if (!strcasecmp(language, "ja")) return "ja";
    if (!strcasecmp(language, "ko")) return "ko";
    if (!strcasecmp(language, "de")) return "de";
    if (!strcasecmp(language, "it")) return "it";
    if (!strcasecmp(language, "nl")) return "nl";
    if (!strcasecmp(language, "fr"))
        return count > 1 && !strcasecmp(parts[1], "ca") ? "fr-CA" : "fr";
    if (!strcasecmp(language, "es"))
        return count > 1 && !strcasecmp(parts[1], "es") ? "es" : "es-419";
    if (!strcasecmp(language, "en")) {
        if (count > 1 && (!strcasecmp(parts[1], "gb") || !strcasecmp(parts[1], "au") ||
                          !strcasecmp(parts[1], "nz") || !strcasecmp(parts[1], "za") ||
                          !strcasecmp(parts[1], "ie"))) return "en-GB";
        return "en-US";
    }
    return "en-US";
}
