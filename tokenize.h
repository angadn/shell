char **tokenize(char *str, char *delims, char **tokens, int n) {
    for (
        char **tok = tokens; 
        (*tok = strsep(&str, delims)) != NULL && tok <= tokens + n;
        **tok == '\0' ? 0 : tok++
    );

    return tokens;
}