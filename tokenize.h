char **tokenize(char *str, char *delims, char **tokens, int n) {
    char *str_tmp = str;
    for (
        char **tok = tokens; 
        (*tok = strsep(&str_tmp, delims)) != NULL && tok <= tokens + n;
        **tok == '\0' ? 0 : tok++
    );

    return tokens;
}