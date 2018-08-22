#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>
#include "zz_config.h"

struct _zzconfig_t {
    char *name;                 //  Property name if any
    char *value;                //  Property value, if any
    int lineno;
    struct _zzconfig_t
            *child,                 //  First child if any
            *next,                  //  Next sibling if any
            *parent;                //  Parent if any
};

//  Local functions for parsing and saving ZPL tokens
static int
        s_collect_level (char **start, int lineno);
static char *
        s_collect_name (char **start, int lineno);
static int
        s_verify_eoln (char *readptr, int lineno);
static char *
        s_collect_value (char **start, int lineno);
static int
        s_config_save (zzconfig_t *self, void *arg, int level);


char *
zz_sys_vprintf (const char *format, va_list argptr)
{
    int size = 1024;
    char *string = (char *) malloc (size);
    int required = vsnprintf (string, size, format, argptr);
    //  If formatted string cannot fit into small string, reallocate a
    //  larger buffer for it.
    if (required >= size) {
        size = required + 1;
        string = (char *) realloc (string, size);
        if (!string)
            return NULL;
        vsnprintf (string, size, format, argptr);
    }
    return string;
}

//  --------------------------------------------------------------------------
//  Constructor
//
//  Optionally attach new config to parent config, as first or next child.

zzconfig_t *
zz_config_new (char *name, zzconfig_t *parent)
{
    zzconfig_t *self = (zzconfig_t *) malloc (sizeof (struct _zzconfig_t));
    memset(self, 0, sizeof (struct _zzconfig_t));
    zz_config_set_name (self, name);
    if (parent) {
        if (parent->child) {
            //  Attach as last child of parent
            zzconfig_t *last = parent->child;
            while (last->next)
                last = last->next;
            last->next = self;
        }
        else
                //  Attach as first child of parent
            parent->child = self;
    }
    self->parent = parent;
    return self;
}


//  --------------------------------------------------------------------------
//  Destructor

void
zz_config_destroy (zzconfig_t **self_p)
{
    assert (self_p);
    if (*self_p) {
        zzconfig_t *self = *self_p;

        //  Destroy all children and siblings recursively
        if (self->child)
            zz_config_destroy (&self->child);
        if (self->next)
            zz_config_destroy (&self->next);
        free (self->name);
        free (self->value);
        free (self);
        *self_p = NULL;
    }
}


//  --------------------------------------------------------------------------
//  Return name of config item

char *
zz_config_name (zzconfig_t *self)
{
    assert (self);
    return self->name;
}


//  --------------------------------------------------------------------------
//  Return value of config item

char *
zz_config_value (zzconfig_t *self)
{
    assert (self);
    return self->value;
}


//  --------------------------------------------------------------------------
//  Insert or update configuration key with value; leading slash is optional 
//  and ignored.

void
zz_config_put (zzconfig_t *self, char *path, char *value)
{
    if (*path == '/')
        path++;

    //  Check length of next path segment
    char *slash = strchr (path, '/');
    int length = strlen (path);
    if (slash)
        length = slash - path;

    //  Find or create items starting at first child of root
    zzconfig_t *child = self->child;
    while (child) {
        if (strlen (child->name) == length
                &&  memcmp (child->name, path, length) == 0) {
            //  This segment exists
            if (slash)          //  Recurse to next level
                zz_config_put (child, slash + 1, value);
            else
                zz_config_set_value (child, "%s", value);
            return;
        }
        child = child->next;
    }
    //  This segment doesn't exist, create it
    child = zz_config_new (path, self);
    child->name [length] = 0;
    if (slash)                  //  Recurse down further
        zz_config_put (child, slash, value);
    else
        zz_config_set_value (child, "%s", value);
}


//  --------------------------------------------------------------------------
//  Set new name for config item; this may be null.

void
zz_config_set_name (zzconfig_t *self, char *name)
{
    assert (self);
    free (self->name);
    self->name = name? strdup (name): NULL;
}


//  --------------------------------------------------------------------------
//  Set new value for config item. The new value may be a string, a printf
//  format, or NULL. Note that if string may possibly contain '%', or if it
//  comes from an insecure source, you must use '%s' as the format, followed
//  by the string.

void
zz_config_set_value (zzconfig_t *self, char *format, ...)
{
    assert (self);
    free (self->value);
    if (format) {
        va_list argptr;
        va_start (argptr, format);
        self->value = zz_sys_vprintf (format, argptr);
        va_end (argptr);
    }
    else
        self->value = NULL;
}


//  --------------------------------------------------------------------------
//  Find our first child, if any

zzconfig_t *
zz_config_child (zzconfig_t *self)
{
    assert (self); 
    return self->child;
}


//  --------------------------------------------------------------------------
//  Find our first sibling, if any

zzconfig_t *
zz_config_next (zzconfig_t *self)
{
    assert (self);
    return self->next;
}


//  --------------------------------------------------------------------------
//  Find a config item along a path; leading slash is optional and ignored.

zzconfig_t *
zz_config_locate (zzconfig_t *self, char *path)
{
    //  Check length of next path segment
    if (*path == '/')
        path++;
    char *slash = strchr (path, '/');
    int length = strlen (path);
    if (slash)
        length = slash - path;

    //  Find matching name starting at first child of root
    zzconfig_t *child = self->child;
    while (child) {
        if (strlen (child->name) == length
                &&  memcmp (child->name, path, length) == 0) {
            if (slash)          //  Look deeper
                return zz_config_locate (child, slash);
            else
                return child;
        }
        child = child->next;
    }
    return NULL;
}


//  --------------------------------------------------------------------------
//  Resolve a config path into a string value

char *
zz_config_resolve (zzconfig_t *self, char *path, char *default_value)
{
    zzconfig_t *item = zz_config_locate (self, path);
    if (item)
        return zz_config_value (item);
    else
        return (char *) default_value;
}


//  --------------------------------------------------------------------------
//  Finds the latest node at the specified depth, where 0 is the root. If no
//  such node exists, returns NULL.

zzconfig_t *
zz_config_at_depth (zzconfig_t *self, int level)
{
    while (level > 0) {
        if (self->child) {
            self = self->child;
            while (self->next)
                self = self->next;
            level--;
        }
        else
            return NULL;
    }
    return self;
}


//  --------------------------------------------------------------------------
//  Execute a callback for each config item in the tree

static int
s_config_execute (zzconfig_t *self, zconfig_fct2 handler, void *arg, int level)
{
    assert (self);
    int rc = handler (self, arg, level);

    //  Process all children in one go, as a list
    zzconfig_t *child = self->child;
    while (child && !rc) {
        rc = s_config_execute (child, handler, arg, level + 1);
        if (rc == -1)
            break;              //  -1 from callback means end execution
        child = child->next;
    }
    return rc;
}

int
zz_config_execute (zzconfig_t *self, zconfig_fct2 handler, void *arg)
{
    //  Execute top level config at level zero
    assert (self);
    return s_config_execute (self, handler, arg, 0);
}


//  --------------------------------------------------------------------------
//  Dump the config file to stderr for tracing

void
zz_config_dump (zzconfig_t *self)
{
    zz_config_execute (self, s_config_save, stderr);
}


//  --------------------------------------------------------------------------
//  Load a config item tree from a specified ZPL text file
//
//  Here is an example ZPL stream and corresponding config structure:
//
//  context
//      iothreads = 1
//      verbose = 1      #   Ask for a trace
//  main
//      type = zqueue    #  ZMQ_DEVICE type
//      frontend
//          option
//              hwm = 1000
//              swap = 25000000     #  25MB
//          bind = 'inproc://addr1'
//          bind = 'ipc://addr2'
//      backend
//          bind = inproc://addr3
//
//  root                    Down = child
//    |                     Across = next
//    v
//  context-->main
//    |         |
//    |         v
//    |       type=queue-->frontend-->backend
//    |                      |          |
//    |                      |          v
//    |                      |        bind=inproc://addr3
//    |                      v
//    |                    option-->bind=inproc://addr1-->bind=ipc://addr2
//    |                      |
//    |                      v
//    |                    hwm=1000-->swap=25000000
//    v
//  iothreads=1-->verbose=false

int zz_config_getfileline(zzconfig_t *self)
{
	return self->lineno;
}

zzconfig_t *
zz_config_load (char *filename)
{
    FILE *file = fopen (filename, "r");
    if (!file)
        return NULL;            //  File not found, or unreadable

    //  Parse the file line by line
    zzconfig_t *self = zz_config_new ("root", NULL);
    char cur_line [1024];
    int valid = 1;
    int lineno = 0;

    while (fgets (cur_line, 1024, file)) {
        //  Trim line
        int length = strlen (cur_line);
        while (length && isspace ((unsigned char)cur_line [length - 1]))
            cur_line [--length] = 0;

        //  Collect indentation level and name, if any
        lineno++;
        //  Handle whole-line comment if present
        if (cur_line [0] == '#') {
        }
        char *scanner = cur_line;
        int level = s_collect_level (&scanner, lineno);
        if (level == -1) {
            valid = 0;
            break;
        }
	if(level == 1)
	{
		self->lineno++;
	}
        char *name = s_collect_name (&scanner, lineno);
        if (name == NULL) {
            valid = 0;
            break;
        }
        //  If name is not empty, collect property value
        if (*name) {
            char *value = s_collect_value (&scanner, lineno);
            if (value == NULL)
                valid = 0;
            else {
                //  Navigate to parent for this element
                zzconfig_t *parent = zz_config_at_depth (self, level);
                if (parent) {
                    zzconfig_t *item = zz_config_new (name, parent);
                    item->value = value;
                }
                else {
                    fprintf (stderr, "E: (%d) indentation error\n", lineno);
                    free (value);
                    valid = 0;
                }
            }
        }
        else
        if (s_verify_eoln (scanner, lineno))
            valid = 0;

        free (name);
        if (!valid)
            break;
    }
    //  Either the whole ZPL file is valid or none of it is
    if (!valid)
        zz_config_destroy (&self);
    fclose (file);
    return self;
}


//  Count and verify indentation level, -1 means a syntax error

static int
s_collect_level (char **start, int lineno)
{
    char *readptr = *start;
    while (*readptr == ' ')
        readptr++;
    int level = (readptr - *start) / 4;
    if (level * 4 != readptr - *start) {
        fprintf (stderr, "E: (%d) indent 4 spaces at once\n", lineno);
        level = -1;
    }
    *start = readptr;
    return level;
}

//  Collect property name

static int
s_is_namechar (char thischar)
{
    return (isalnum (thischar)
            || thischar == '$'
            || thischar == '-'
            || thischar == '_'
            || thischar == '@'
            || thischar == '.'
            || thischar == '&'
            || thischar == '+'
            || thischar == '/');
}

static char *
s_collect_name (char **start, int lineno)
{
    char *readptr = *start;
    while (s_is_namechar ((char) **start))
        (*start)++;

    size_t length = *start - readptr;
    char *name = (char *) malloc (length + 1);
    memcpy (name, readptr, length);
    name [length] = 0;

    if (length > 0
            && (name [0] == '/' || name [length - 1] == '/')) {
        fprintf (stderr, "E: (%d) '/' not valid at name start or end\n", lineno);
        free (name);
    }
    return name;
}

//  Check there's no junk after value on line, returns 0 if OK else -1.

static int
s_verify_eoln (char *readptr, int lineno)
{
    while (*readptr) {
        if (isspace ((unsigned char) *readptr))
            readptr++;
        else
        if (*readptr == '#')
            break;
        else {
            fprintf (stderr, "E: (%d) invalid syntax '%s'\n",
                    lineno, readptr);
            return -1;
            break;
        }
    }
    return 0;
}

//  Return value for name, or "" - if syntax error, returns NULL.

static char *
s_collect_value (char **start, int lineno)
{
    char *value = NULL;
    char *readptr = *start;
    int rc = 0;

    while (isspace ((unsigned char) *readptr))
        readptr++;

    if (*readptr == '=') {
        readptr++;
        while (isspace ((unsigned char) *readptr))
            readptr++;

        //  If value starts with quote or apost, collect it
        if (*readptr == '"' || *readptr == '\'') {
            char *endquote = strchr (readptr + 1, *readptr);
            if (endquote) {
                size_t value_length = endquote - readptr - 1;
                value = (char *) malloc (value_length + 1);
                memcpy (value, readptr + 1, value_length);
                value [value_length] = 0;
                rc = s_verify_eoln (endquote + 1, lineno);
            }
            else {
                fprintf (stderr, "E: (%d) missing %c\n", lineno, *readptr);
                rc = -1;
            }
        }
        else {
            //  Collect unquoted value up to comment
            char *comment = strchr (readptr, '#');
            if (comment) {
                while (isspace ((unsigned char) comment [-1]))
                    comment--;
                *comment = 0;
            }
            value = strdup (readptr);
        }
    }
    else {
        value = strdup ("");
        rc = s_verify_eoln (readptr, lineno);
    }
    //  If we had an error, drop value and return NULL
    if (rc)
        free (value);
    return value;
}


//  --------------------------------------------------------------------------
//  Add comment to config item before saving to disk. You can add as many
//  comment lines as you like. If you use a null format, all comments are 
//  deleted.




//  --------------------------------------------------------------------------
//  Save a config item tree to a specified ZPL text file, where a filename
//  "-" means dump to standard output.

int
zz_config_save (zzconfig_t *self, char *filename)
{
    assert (self);

    int rc = 0;
    if (strcmp (filename, "-") == 0) {
        //  "-" means write to stdout
        rc = zz_config_execute (self, s_config_save, stdout);
    }
    else {
        FILE *file;
        file = fopen (filename, "w");
        if (file)
            rc = zz_config_execute (self, s_config_save, file);
        else
            rc = -1;          //  File not writeable
        fflush (file);
        fclose (file);
    }
    return rc;
}

//  Save an item

static int
s_config_save (zzconfig_t *self, void *arg, int level)
{
    assert (self);
    assert (arg);

    FILE *file = (FILE *) arg;

    if (level > 0) {
        if (self->value)
            fprintf (file, "%*s%s = \"%s\"\n", (level - 1) * 4, "",
                    self->name? self->name: "(Unnamed)", self->value);
        else
            fprintf (file, "%*s%s\n", (level - 1) * 4, "",
                    self->name? self->name: "(Unnamed)");
    }
    return 0;
}


//  --------------------------------------------------------------------------
//  Self test of this class

int main2 ()
{
    printf ("begin: \n");
    zzconfig_t * root = zz_config_load ("./jc.cfg");
    zzconfig_t * child = zz_config_locate(root, "dst_host");
    child = zz_config_child(child);
    while (child)
    {
        char * host = zz_config_value(child);
        char * uri = zz_config_resolve(child, "uri", "");
        printf("host:%s, uri:%s\n", host, uri);

        zzconfig_t * child2 = zz_config_locate(child, "dst");
        child2 = zz_config_child(child2);
        while (child2)
        {
            char * dst_url = zz_config_value(child2);
            printf("    dst:%s\n", dst_url);

            char * old = zz_config_resolve(child2, "count", "none");
            printf("        count:%s\n", old);

            zz_config_put(child2, "count", "143999:99,99,99");
            child2 = zz_config_next(child2);
        }
        child = zz_config_next(child);
    }

    zz_config_save(root, "jc2.cfg");

//    zzconfig_t *root = zz_config_new ("root", NULL);
//    zzconfig_t *section, *item;
//
//
//
//    section = zz_config_new ("headers", root);
//    item = zz_config_new ("email", section);
//    zz_config_set_value (item, "some@random.com");
//    item = zz_config_new ("name", section);
//    zz_config_set_value (item, "Justin Kayce");
//    zz_config_put (root, "/curve/secret-key", "Top Secret");
//    zz_config_save (root, "./test.cfg");
//    zz_config_destroy (&root);
//    root = zz_config_load ("./test.cfg");
//
//    zz_config_save (root, "-");
//
//    char *email = zz_config_resolve (root, "/headers/email", NULL);
//    printf(email);
//    char *passwd = zz_config_resolve (root, "/curve/secret-key", NULL);
//    printf(passwd);
//
//    zz_config_save (root, "./test.cfg");
//    zz_config_destroy (&root);

    printf ("OK\n");
}

