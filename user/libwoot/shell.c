#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <woot/shell.h>
#include <woot/vector.h>

typedef struct
{
    regex_t Regex;
    char *Editor;
} shellEditor_t;

static const char *editorsFile = "/etc/woot/editors.cfg";
static vecVector_t *editors;

int shellInitialize()
{
    editors = vecCreate(sizeof(shellEditor_t), 64, 16, __INT_MAX__);
    FILE *f = fopen(editorsFile, "rb");
    if(f)
    {
        char *line = NULL;
        size_t size = 0;
        while((getline(&line, &size, f)) >= 0)
        {
            char *comment = strchr(line, '#');
            if(comment) *comment = 0;
            char *nl = strrchr(line, '\n');
            if(nl) *nl = 0;

            char *command = strrchr(line, ':');
            if(!command)
                continue; // invalid line (skip)
            *command++ = 0;
            regex_t regex;
            if(regcomp(&regex, line, REG_EXTENDED | REG_ICASE))
                continue; // invalid regex (skip)
            shellEditor_t editor = { regex, strdup(command) };
            vecAppend(editors, &editor);
        }
        if(line) free(line);
        fclose(f);
    }
    return 0;
}

int shellCleanup()
{
    if(editors)
    {
        unsigned edCnt = vecSize(editors);
        for(unsigned i = 0; i < edCnt; ++i)
        {
            shellEditor_t *ed = vecGet(editors, i);
            if(!ed) continue;
            regfree(&ed->Regex);
            if(ed->Editor) free(ed->Editor);
        }
        vecDelete(editors);
    }
    return 0;
}

const char *shellGetEditor(const char *filename, int idx)
{
    size_t len = strlen(filename);
    unsigned edCnt = vecSize(editors);
    for(unsigned i = 0; i < edCnt; ++i)
    {
        shellEditor_t *ed = vecGet(editors, i);
        if(!ed) continue;
        regmatch_t match;
        if(regexec(&ed->Regex, filename, 1, &match, 0) != 0)
            continue; // no match
        if(match.rm_so || match.rm_eo != len)
            continue; // not full match

        if(!idx--)
            return ed->Editor;
    }
    return NULL;
}
