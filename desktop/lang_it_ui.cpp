#include <gtk/gtk.h>
#include <string>
#include "../lang_it.h"

static void on_translate(GtkWidget *widget, gpointer data) {
    GtkEntry* entry = GTK_ENTRY(((GtkWidget**)data)[0]);
    GtkLabel* label = GTK_LABEL(((GtkWidget**)data)[1]);
    GtkComboBoxText* from_combo = GTK_COMBO_BOX_TEXT(((GtkWidget**)data)[2]);
    GtkComboBoxText* to_combo = GTK_COMBO_BOX_TEXT(((GtkWidget**)data)[3]);

    const char* input_text = gtk_entry_get_text(entry);
    const char* from_lang = gtk_combo_box_text_get_active_text(from_combo);
    const char* to_lang   = gtk_combo_box_text_get_active_text(to_combo);

    if (input_text && *input_text && from_lang && to_lang) {
        const char* from_code = nullptr;
        const char* to_code = nullptr;

        if (strcmp(from_lang, "Portuguese") == 0) from_code = "pt";
        else if (strcmp(from_lang, "English") == 0) from_code = "en";

        if (strcmp(to_lang, "English") == 0) to_code = "en";
        else if (strcmp(to_lang, "Japanese") == 0) to_code = "ja";
        else if (strcmp(to_lang, "Spanish") == 0) to_code = "es";

        if (from_code && to_code) {
            std::string output = translate(input_text, from_code, to_code);
            gtk_label_set_text(label, output.c_str());
        } else {
            gtk_label_set_text(label, "Unsupported translation");
        }
    } else {
        gtk_label_set_text(label, "");  
    }

    if (from_lang) g_free((gchar*)from_lang);
    if (to_lang) g_free((gchar*)to_lang);
}

// Update "to" combo box depending on the selected "from" language
static void on_choose(GtkComboBoxText *combo, gpointer data) {
    GtkComboBoxText* to_combo = GTK_COMBO_BOX_TEXT(data);
    const char* from_lang = gtk_combo_box_text_get_active_text(combo);

    gtk_combo_box_text_remove_all(to_combo);

    if (!from_lang) return;

    if (strcmp(from_lang, "Portuguese") == 0) {
        gtk_combo_box_text_append_text(to_combo, "English");
        gtk_combo_box_text_append_text(to_combo, "Spanish");
    } else if (strcmp(from_lang, "English") == 0) {
        gtk_combo_box_text_append_text(to_combo, "Japanese");
        gtk_combo_box_text_append_text(to_combo, "Spanish");
    }

    gtk_combo_box_set_active(GTK_COMBO_BOX(to_combo), 0);
    g_free((gchar*)from_lang);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "lang_it gtk");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 200);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

    GtkWidget *entry = gtk_entry_new();
    GtkWidget *from = gtk_combo_box_text_new();
    GtkWidget *to   = gtk_combo_box_text_new();

    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(from), "Portuguese");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(from), "English");

    gtk_combo_box_set_active(GTK_COMBO_BOX(from), 0);
    on_choose(GTK_COMBO_BOX_TEXT(from), to);

    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Enter text");

    GtkWidget *button = gtk_button_new_with_label("Translate");
    GtkWidget *label  = gtk_label_new("");

    gtk_box_pack_start(GTK_BOX(hbox), from, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(hbox), to, FALSE, FALSE, 5);

    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 5);

    gtk_container_add(GTK_CONTAINER(window), vbox);

    GtkWidget* widgets[4] = {entry, label, from, to};
    g_signal_connect(button, "clicked", G_CALLBACK(on_translate), widgets);
    g_signal_connect(entry, "changed", G_CALLBACK(on_translate), widgets);
    g_signal_connect(from, "changed", G_CALLBACK(on_choose), to);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
