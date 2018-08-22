
#ifndef __CONFIG_INCLUDED__
#define __CONFIG_INCLUDED__

//  Opaque class structure
typedef struct _zzconfig_t zzconfig_t;

//  Function that executes config
typedef int (zconfig_fct2) (zzconfig_t *self, void *arg, int level);

//  Create new config item
 zzconfig_t * zz_config_new (char *name, zzconfig_t *parent);

//  Destroy a config item and all its children
 void zz_config_destroy (zzconfig_t **self_p);

//  Return name of config item
 char * zz_config_name (zzconfig_t *self);

//  Return value of config item
 char * zz_config_value (zzconfig_t *self);

//  Insert or update configuration key with value
 void zz_config_put (zzconfig_t *self, char *path, char *value);

//  Set config item name, name may be NULL
 void zz_config_set_name (zzconfig_t *self, char *name);

//  Set new value for config item. The new value may be a string, a printf
//  format, or NULL. Note that if string may possibly contain '%', or if it
//  comes from an insecure source, you must use '%s' as the format, followed
//  by the string.
 void zz_config_set_value (zzconfig_t *self, char *format, ...);

//  Find our first child, if any
 zzconfig_t * zz_config_child (zzconfig_t *self);

//  Find our first sibling, if any
 zzconfig_t * zz_config_next (zzconfig_t *self);

//  Find a config item along a path; leading slash is optional and ignored.
 zzconfig_t * zz_config_locate (zzconfig_t *self, char *path);

//  Resolve a config path into a string value; leading slash is optional 
//  and ignored.
 char * zz_config_resolve (zzconfig_t *self, char *path, char *default_value);

//  Set config item name, name may be NULL
 void zz_config_set_path (zzconfig_t *self, char *path, char *value);

//  Locate the last config item at a specified depth
 zzconfig_t * zz_config_at_depth (zzconfig_t *self, int level);

//  Execute a callback for each config item in the tree
 int zz_config_execute (zzconfig_t *self, zconfig_fct2 handler, void *arg);

//  Get load file line sum
 int zz_config_getfileline(zzconfig_t *self);

//  Load a config item tree from a specified ZPL text file
 zzconfig_t * zz_config_load (char *filename);

//  Save a config item tree to a specified ZPL text file, where a filename
//  "-" means dump to standard output.
 int zz_config_save (zzconfig_t *self, char *filename);

//  Dump the config file to stderr for tracing
 void zz_config_dump (zzconfig_t *self);



#endif
