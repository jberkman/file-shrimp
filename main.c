/*
 * Initial main.c file generated by Glade. Edit as required.
 * Glade will not overwrite this file.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <libhal-storage.h>

#include <string.h>

#if 0
#include <dbus/dbus-glib.h>
#include <dbus/dbus-glib-lowlevel.h>
#endif

#include "callbacks.h"
#include "interface.h"
#include "support.h"

#define DBUS_TIMEOUT G_MAXINT

enum {
    COL_ICON,
    COL_LABEL,
    COL_DRIVE,
    N_COLS
};

typedef struct {
    char *udi;
    char *version;
    char *partitions;
} ShrimpDrive;

static GtkListStore *target_drives = NULL;

#if 1
static LibHalStoragePolicy *policy = NULL;

/* from gnome-vfs-hal-mounts.c */
/* by design, the enums are laid out so we can do easy computations */
static LibHalStoragePolicyIconPair hal_icon_mapping[] = {
	{LIBHAL_STORAGE_ICON_DRIVE_REMOVABLE_DISK,           "gnome-dev-removable"},
	{LIBHAL_STORAGE_ICON_DRIVE_REMOVABLE_DISK_IDE,       "gnome-dev-removable"},
	{LIBHAL_STORAGE_ICON_DRIVE_REMOVABLE_DISK_SCSI,      "gnome-dev-removable"},
	{LIBHAL_STORAGE_ICON_DRIVE_REMOVABLE_DISK_USB,       "gnome-dev-removable-usb"},
	{LIBHAL_STORAGE_ICON_DRIVE_REMOVABLE_DISK_IEEE1394,  "gnome-dev-removable-1394"},
#ifdef LIBHAL_STORAGE_ICON_DRIVE_REMOVABLE_DISK_CCW
	{LIBHAL_STORAGE_ICON_DRIVE_REMOVABLE_DISK_CCW,       "gnome-dev-removable"},
#endif
	{LIBHAL_STORAGE_ICON_DRIVE_DISK,                     "gnome-dev-removable"},
	{LIBHAL_STORAGE_ICON_DRIVE_DISK_IDE,                 "gnome-dev-removable"},
	{LIBHAL_STORAGE_ICON_DRIVE_DISK_SCSI,                "gnome-dev-removable"},       /* TODO: gnome-dev-removable-scsi */
	{LIBHAL_STORAGE_ICON_DRIVE_DISK_USB,                 "gnome-dev-removable-usb"},
	{LIBHAL_STORAGE_ICON_DRIVE_DISK_IEEE1394,            "gnome-dev-removable-1394"},
#ifdef LIBHAL_STORAGE_ICON_DRIVE_DISK_CCW
	{LIBHAL_STORAGE_ICON_DRIVE_DISK_CCW,                 "gnome-dev-removable"},
#endif
	{LIBHAL_STORAGE_ICON_DRIVE_CDROM,                    "gnome-dev-removable"},       /* TODO: gnome-dev-removable-cdrom */
#ifdef LIBHAL_STORAGE_ICON_DRIVE_CDWRITER
	{LIBHAL_STORAGE_ICON_DRIVE_CDWRITER,                 "gnome-dev-removable"},       /* TODO: gnome-dev-removable-cdwriter */
#endif
	{LIBHAL_STORAGE_ICON_DRIVE_FLOPPY,                   "gnome-dev-removable"},       /* TODO: gnome-dev-removable-floppy */
	{LIBHAL_STORAGE_ICON_DRIVE_TAPE,                     "gnome-dev-removable"},       /* TODO: gnome-dev-removable-tape */
	{LIBHAL_STORAGE_ICON_DRIVE_COMPACT_FLASH,            "gnome-dev-removable"},       /* TODO: gnome-dev-removable-cf */
	{LIBHAL_STORAGE_ICON_DRIVE_MEMORY_STICK,             "gnome-dev-removable"},       /* TODO: gnome-dev-removable-ms */
	{LIBHAL_STORAGE_ICON_DRIVE_SMART_MEDIA,              "gnome-dev-removable"},       /* TODO: gnome-dev-removable-sm */
	{LIBHAL_STORAGE_ICON_DRIVE_SD_MMC,                   "gnome-dev-removable"},       /* TODO: gnome-dev-removable-sdmmc */
	{LIBHAL_STORAGE_ICON_DRIVE_CAMERA,                   "gnome-dev-removable"},       /* TODO: gnome-dev-removable-camera */
	{LIBHAL_STORAGE_ICON_DRIVE_PORTABLE_AUDIO_PLAYER,    "gnome-dev-removable"},       /* TODO: gnome-dev-removable-ipod */
	{LIBHAL_STORAGE_ICON_DRIVE_ZIP,                      "gnome-dev-removable"},       /* TODO: gnome-dev-removable-zip */
	{LIBHAL_STORAGE_ICON_DRIVE_JAZ,                      "gnome-dev-removable"},       /* TODO: gnome-dev-removable-jaz */
	{LIBHAL_STORAGE_ICON_DRIVE_FLASH_KEY,                "gnome-dev-removable"},       /* TODO: gnome-dev-removable-pendrive */

	{LIBHAL_STORAGE_ICON_VOLUME_REMOVABLE_DISK,          "gnome-dev-harddisk"},
	{LIBHAL_STORAGE_ICON_VOLUME_REMOVABLE_DISK_IDE,      "gnome-dev-harddisk"},
	{LIBHAL_STORAGE_ICON_VOLUME_REMOVABLE_DISK_SCSI,     "gnome-dev-harddisk"},        /* TODO: gnome-dev-harddisk-scsi */
	{LIBHAL_STORAGE_ICON_VOLUME_REMOVABLE_DISK_USB,      "gnome-dev-harddisk-usb"},
	{LIBHAL_STORAGE_ICON_VOLUME_REMOVABLE_DISK_IEEE1394, "gnome-dev-harddisk-1394"},
#ifdef LIBHAL_STORAGE_ICON_VOLUME_REMOVABLE_DISK_CCW
	{LIBHAL_STORAGE_ICON_VOLUME_REMOVABLE_DISK_CCW,      "gnome-dev-harddisk"},
#endif
	{LIBHAL_STORAGE_ICON_VOLUME_DISK,                    "gnome-dev-harddisk"},
	{LIBHAL_STORAGE_ICON_VOLUME_DISK_IDE,                "gnome-dev-harddisk"},
	{LIBHAL_STORAGE_ICON_VOLUME_DISK_SCSI,               "gnome-dev-harddisk"},
	{LIBHAL_STORAGE_ICON_VOLUME_DISK_USB,                "gnome-dev-harddisk-usb"},
	{LIBHAL_STORAGE_ICON_VOLUME_DISK_IEEE1394,           "gnome-dev-harddisk-1394"},
#ifdef LIBHAL_STORAGE_ICON_VOLUME_DISK_CCW
	{LIBHAL_STORAGE_ICON_VOLUME_DISK_CCW,                "gnome-dev-harddisk"},
#endif
	{LIBHAL_STORAGE_ICON_VOLUME_FLOPPY,                  "gnome-dev-floppy"},
	{LIBHAL_STORAGE_ICON_VOLUME_TAPE,                    "gnome-dev-harddisk"},
	{LIBHAL_STORAGE_ICON_VOLUME_COMPACT_FLASH,           "gnome-dev-media-cf"},
	{LIBHAL_STORAGE_ICON_VOLUME_MEMORY_STICK,            "gnome-dev-media-ms"},
	{LIBHAL_STORAGE_ICON_VOLUME_SMART_MEDIA,             "gnome-dev-media-sm"},
	{LIBHAL_STORAGE_ICON_VOLUME_SD_MMC,                  "gnome-dev-media-sdmmc"},
	{LIBHAL_STORAGE_ICON_VOLUME_CAMERA,                  "camera"},
	{LIBHAL_STORAGE_ICON_VOLUME_PORTABLE_AUDIO_PLAYER,   "gnome-dev-ipod"},
	{LIBHAL_STORAGE_ICON_VOLUME_ZIP,                     "gnome-dev-zipdisk"},
	{LIBHAL_STORAGE_ICON_VOLUME_JAZ,                     "gnome-dev-jazdisk"},
	{LIBHAL_STORAGE_ICON_VOLUME_FLASH_KEY,               "gnome-dev-harddisk"},        /* TODO: gnome-dev-pendrive */

	{LIBHAL_STORAGE_ICON_DISC_CDROM,                     "gnome-dev-cdrom"},
	{LIBHAL_STORAGE_ICON_DISC_CDR,                       "gnome-dev-disc-cdr"},
	{LIBHAL_STORAGE_ICON_DISC_CDRW,                      "gnome-dev-disc-cdrw"},
	{LIBHAL_STORAGE_ICON_DISC_DVDROM,                    "gnome-dev-disc-dvdrom"},
	{LIBHAL_STORAGE_ICON_DISC_DVDRAM,                    "gnome-dev-disc-dvdram"},
	{LIBHAL_STORAGE_ICON_DISC_DVDR,                      "gnome-dev-disc-dvdr"},
	{LIBHAL_STORAGE_ICON_DISC_DVDRW,                     "gnome-dev-disc-dvdrw"},
	{LIBHAL_STORAGE_ICON_DISC_DVDPLUSR,                  "gnome-dev-disc-dvdr-plus"},
	{LIBHAL_STORAGE_ICON_DISC_DVDPLUSRW,                 "gnome-dev-disc-dvdrw"},      /* TODO: gnome-dev-disc-dvdrw-plus */
#ifdef LIBHAL_STORAGE_ICON_DISC_DVDPLUSR_DL
	{LIBHAL_STORAGE_ICON_DISC_DVDPLUSR_DL,               "gnome-dev-disc-dvdr-plus"},  /* TODO: gnome-dev-disc-dvdr-plus-dl */
#endif

	{0x00, NULL}
};
#endif

static char *
get_mount_version (const char *path, GError **gerror)
{
    char *file;
    char *contents;
    gsize length;
    char *ret;

    file = g_build_filename (path, ".profile", NULL);
    if (!g_file_get_contents (file, &contents, &length, gerror)) {
        return NULL;
    }

#define VERSTR "version=\""
    ret = strcasestr (contents, VERSTR);
    if (!ret) {
        return NULL;
    }

    ret += strlen (VERSTR);
#undef VERSTR

    length = strchr (ret, '"') - ret;
    ret = g_strndup (ret, length);

    return ret;
}

#if 0
#define HAS_GET_VOLUME_VERSION 1
static char *
get_volume_version (LibHalVolume *vol, DBusError *error)
{
    DBusMessage *dmesg       = NULL;
    DBusMessage *reply       = NULL;
    char        *mount_point = "tmp";
    char        *fstype      = "";
    char        *ret         = NULL;
    GError      *gerror      = NULL;
    const char **options;
    const char  *udi;
    DBusConnection *conn;

    udi = libhal_volume_get_udi (vol);

    options = g_new0 (const char *, 1);
    options[0] = "ro";

    if (!(dmesg = dbus_message_new_method_call ("org.freedesktop.Hal", udi,
                                                "org.freedesktop.Hal.Device.Volume",
                                                "Mount"))) {
        g_warning ("Could not create dbus message for %s", udi);
        goto leave;
    }
            
    if (!dbus_message_append_args (dmesg,
                                   DBUS_TYPE_STRING, &mount_point,
                                   DBUS_TYPE_STRING, &fstype,
                                   DBUS_TYPE_ARRAY, DBUS_TYPE_STRING, &options, 1,
                                   DBUS_TYPE_INVALID)) {
        g_warning ("Could not append args to dbus message for %s", udi);
        goto leave;
    }
    
    conn = dbus_bus_get (DBUS_BUS_SYSTEM, error);

    if (!(reply = dbus_connection_send_with_reply_and_block (conn, dmesg, DBUS_TIMEOUT, error)) ||
        dbus_error_is_set (error)) {
        goto leave;
    }
    
    dbus_message_unref (dmesg);
    dbus_message_unref (reply);
    reply = NULL;

    ret = get_mount_version ("/media/tmp", &gerror);

    dmesg = dbus_message_new_method_call ("org.freedesktop.Hal", udi,
                                          "org.freedesktop.Hal.Device.Volume",
                                          "Unmount");
    if (dmesg == NULL) {
        g_warning ("Could not create dbus message for %s", udi);
        goto leave;
    }
    
    if (!dbus_message_append_args (dmesg, 
                                   DBUS_TYPE_ARRAY, DBUS_TYPE_STRING, &options, 0,
                                   DBUS_TYPE_INVALID)) {
        g_warning ("Could not append args to dbus message for %s", udi);
        goto leave;
    }
    
    reply = dbus_connection_send_with_reply_and_block (conn, dmesg, DBUS_TIMEOUT, error);

    leave:
    g_free (options);
    if (dmesg) {
        dbus_message_unref (dmesg);
    }
    if (reply) {
        dbus_message_unref (reply);
    }
    return ret;
}
#endif

static ShrimpDrive *
add_storage_udi (LibHalContext *hal_ctx, const char *udi, DBusError *error)
{
    GtkTreeIter iter;
    ShrimpDrive *ret = NULL;
    LibHalDrive *drive;
    char **vols;
    int n_vols;
    int j;
    LibHalVolume *vol;
    GString *partitions;
    char *tmp;

    drive = libhal_drive_from_udi (hal_ctx, udi);

    switch (libhal_drive_get_type (drive)) {
    case LIBHAL_DRIVE_TYPE_CDROM:
    case LIBHAL_DRIVE_TYPE_FLOPPY:
    case LIBHAL_DRIVE_TYPE_TAPE:
        goto next_drive;
    }
    
    ret = g_new0 (ShrimpDrive, 1);
    partitions = g_string_new (NULL);

    ret->udi = g_strdup (udi);
#if 0
    label = g_strdup_printf ("%s %s",
                             libhal_drive_get_vendor (drive),
                             libhal_drive_get_model  (drive));
#endif

    vols = libhal_drive_find_all_volumes (hal_ctx, drive, &n_vols);
    printf ("%s has %d volumes\n", udi, n_vols);
    
    for (j = 0; j < n_vols; j++) {
        vol = libhal_volume_from_udi (hal_ctx, vols[j]);

        if (partitions->len) {
            g_string_append (partitions, ", ");
        }
        tmp = libhal_volume_policy_compute_size_as_string (vol);
        g_print ("partition %d: %s\n", j, tmp);
        g_string_append (partitions, tmp);
        g_free (tmp);

        if (libhal_volume_is_mounted (vol) ||
            libhal_volume_should_ignore (vol)) {
            goto next_vol;
        }
        
#ifdef HAS_GET_VOLUME_VERSION
        ret->version = get_volume_version (vol, error);
        if (dbus_error_is_set (error)) {
            g_warning ("Error trying to get version: %s %s", error->name, error->message);
            dbus_error_free (error);
            dbus_error_init (error);
        }
#endif
    next_vol:
        libhal_volume_free (vol);
        if (ret->version) {
            g_print ("Found version %s\n", ret->version);
            break;
        }
    }
    
    ret->partitions = g_string_free (partitions, FALSE);

next_vols:
    gtk_list_store_append (target_drives, &iter);
    gtk_list_store_set (target_drives, &iter,
                        COL_ICON,  libhal_drive_policy_compute_icon_name (drive, NULL, policy),
                        COL_LABEL, libhal_drive_policy_compute_display_name (drive, NULL, policy),
                        COL_DRIVE, ret,
                        -1);

#if 0
    g_free (label);
#endif
    libhal_free_string_array (vols);

next_drive:
    libhal_drive_free (drive);
    return ret;
}

typedef struct {
    LibHalContext *ctx;
    char *udi;
} AddedData;

static gboolean
device_added_idle_cb (gpointer data)
{
    AddedData *ad = data;

    add_storage_udi (ad->ctx, ad->udi, NULL);

    g_free (ad->udi);
    g_free (ad);

    return FALSE;
}

static void
device_added_cb (LibHalContext *ctx,
                 const char    *udi)
{
    AddedData *ad;
    if (!libhal_device_query_capability (ctx, udi, "storage", NULL)) {
        return;
    }
    ad = g_new (AddedData, 1);
    ad->ctx = ctx;
    ad->udi = g_strdup (udi);
    g_idle_add (device_added_idle_cb, ad);
}

static gboolean
device_removed_foreach_cb (GtkTreeModel *model,
                           GtkTreePath  *path,
                           GtkTreeIter  *iter,
                           gpointer      data)
{
    const char *udi = data;
    ShrimpDrive *drive = NULL;

    gtk_tree_model_get (model, iter, COL_DRIVE, &drive, -1);
    if (!drive || strcmp (drive->udi, udi)) {
        return FALSE;
    }
    gtk_list_store_remove (GTK_LIST_STORE (model), iter);
    g_print ("free: %p\n", drive);
    return TRUE;
}

static void
device_removed_cb (LibHalContext *ctx,
                   const char    *udi)
{
    GtkWidget *shrimp;
    GtkWidget *w;
    GtkTreeModel *model;

    shrimp = libhal_ctx_get_user_data (ctx);
    w = lookup_widget (shrimp, "drives_combo");
    model = gtk_combo_box_get_model (GTK_COMBO_BOX (w));

    gtk_tree_model_foreach (model, device_removed_foreach_cb, (char *)udi);
    on_drives_combo_changed (GTK_COMBO_BOX (w), NULL);
}

static gboolean
populate_targets (gpointer data)
{
    DBusError error;
    LibHalContext *hal_ctx;

    int i;
    int num_udis;
    char **udis;

    DBusConnection *conn;
    GtkTreeIter iter;
    GtkWidget *shrimp = data;
    GtkWidget *w;

    if ((hal_ctx = libhal_ctx_new ()) == NULL) {
        fprintf (stderr, "error: libhal_ctx_new\n");
        return FALSE;
    }

    libhal_ctx_set_user_data (hal_ctx, shrimp);

    dbus_error_init (&error);
    conn = dbus_bus_get (DBUS_BUS_SYSTEM, &error);

    dbus_connection_setup_with_g_main (conn, NULL);
    dbus_connection_set_exit_on_disconnect (conn, FALSE);

    if (!libhal_ctx_set_dbus_connection (hal_ctx, conn)) {
        fprintf (stderr, "error: libhal_ctx_set_dbus_connection: %s: %s\n", error.name, error.message);
        return FALSE;
    }

    libhal_ctx_set_device_added (hal_ctx, device_added_cb);
    libhal_ctx_set_device_removed (hal_ctx, device_removed_cb);

#if 0
    libhal_device_property_watch_all (hal_ctx, &error);
#endif

    if (!libhal_ctx_init (hal_ctx, &error)) {
        fprintf (stderr, "error: libhal_ctx_init: %s: %s\n", error.name, error.message);
        return FALSE;
    }
   
    udis = libhal_find_device_by_capability (hal_ctx, "storage", &num_udis, &error);
    
    if (dbus_error_is_set (&error)) {
        fprintf (stderr, "error: %s: %s\n", error.name, error.message);
        return FALSE;
    }

    for (i = 0; i < num_udis; i++) {
        add_storage_udi (hal_ctx, udis[i], &error);
    }

    if (gtk_tree_model_get_iter_first (GTK_TREE_MODEL (target_drives), &iter)) {
        w = lookup_widget (shrimp, "drives_combo");
        gtk_combo_box_set_active_iter (GTK_COMBO_BOX (w), &iter);
    }

    libhal_free_string_array (udis);

    return FALSE;
}

void
on_drives_combo_changed                (GtkComboBox     *combobox,
                                        gpointer         user_data)
{
    GtkTreeIter   iter;
    GtkWidget    *shrimp;
    GtkWidget    *w;
    ShrimpDrive  *drive = NULL;
    GtkTreeModel *model;

    shrimp = gtk_widget_get_toplevel (GTK_WIDGET (combobox));
    model = gtk_combo_box_get_model (combobox);

    if (gtk_combo_box_get_active_iter (combobox, &iter)) {
        gtk_tree_model_get (model, &iter,
                            COL_DRIVE, &drive,
                            -1);
    } else if (gtk_tree_model_get_iter_first (model, &iter)) {
        gtk_combo_box_set_active_iter (combobox, &iter);
        return;
    }

    w = lookup_widget (shrimp, "target_version");
    gtk_label_set_text (GTK_LABEL (w), drive && drive->version ? drive->version : "SLE Thin Client not installed on target");

    w = lookup_widget (shrimp, "target_partitions");
    gtk_label_set_text (GTK_LABEL (w), drive && drive->partitions ? drive->partitions : "none");
}

int
main (int argc, char *argv[])
{
  GtkWidget *shrimp;
  GtkWidget *w;
  GtkCellRenderer *col;
  char *root_version;

#ifdef ENABLE_NLS
  bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
  bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
  textdomain (GETTEXT_PACKAGE);
#endif

  gtk_set_locale ();
  gtk_init (&argc, &argv);

  add_pixmap_directory (PACKAGE_DATA_DIR "/" PACKAGE "/pixmaps");

#if 1
  policy = libhal_storage_policy_new ();
  libhal_storage_policy_set_icon_mapping (policy, hal_icon_mapping);
#endif

  target_drives = gtk_list_store_new (N_COLS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_POINTER, -1);

  /*
   * The following code was added by Glade to create one of each component
   * (except popup menus), just so that you see something after building
   * the project. Delete any components that you don't want shown initially.
   */
  shrimp = create_shrimp ();

  root_version = get_mount_version ("/", NULL);
  if (root_version) {
      w = lookup_widget (shrimp, "current_version");
      gtk_label_set_text (GTK_LABEL (w), root_version);
  } else {
      w = lookup_widget (shrimp, "current_version");
      gtk_label_set_markup (GTK_LABEL (w), "<b>No version of SLE Thin Client found</b>");
      w = lookup_widget (shrimp, "install_button");
      gtk_widget_set_sensitive (w, FALSE);
  }

  w = lookup_widget (shrimp, "drives_combo");
  gtk_cell_layout_clear (GTK_CELL_LAYOUT (w));

  col = g_object_new (GTK_TYPE_CELL_RENDERER_PIXBUF, NULL);
  gtk_cell_layout_pack_start (GTK_CELL_LAYOUT (w), col, FALSE);
  gtk_cell_layout_add_attribute (GTK_CELL_LAYOUT (w), col, "icon-name", COL_ICON);

  col = g_object_new (GTK_TYPE_CELL_RENDERER_TEXT, NULL);
  gtk_cell_layout_pack_start (GTK_CELL_LAYOUT (w), col, TRUE);
  gtk_cell_layout_add_attribute (GTK_CELL_LAYOUT (w), col, "text", COL_LABEL);

  gtk_combo_box_set_model (GTK_COMBO_BOX (w), GTK_TREE_MODEL (target_drives));

  gtk_widget_show (shrimp);

  g_idle_add (populate_targets, shrimp);

  gtk_main ();
  return 0;
}
