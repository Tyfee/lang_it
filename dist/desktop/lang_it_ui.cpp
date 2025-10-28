#include <gtk/gtk.h>
#include <string>
#include "../../src/lang_it.h"
#include <pango/pango.h>


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


std::string getKanji(int offset) {
    gunichar base = 0x4E00;
    gunichar ch = base + offset;

    gchar utf8[6];
    gint len = g_unichar_to_utf8(ch, utf8);
    utf8[len] = '\0';

    return std::string(utf8);
}


void on_settings(GtkWidget *widget, gpointer user_data) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Settings",
        GTK_WINDOW(user_data),  
        GTK_DIALOG_MODAL,
        "_Close", GTK_RESPONSE_CLOSE,
        NULL
    );

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *label = gtk_label_new("Settings go here...");
    gtk_container_add(GTK_CONTAINER(content_area), label);

    gtk_widget_show_all(dialog);

    g_signal_connect_swapped(dialog, "response", G_CALLBACK(gtk_widget_destroy), dialog);
}

void on_open_file(GtkWidget *widget, gpointer user_data) {
}

void on_manage_dictionaries(GtkWidget *widget, gpointer user_data) {
    
}

int main(int argc, char *argv[]) {

 


    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "lang_it gtk");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);


    GtkWidget* navbar = gtk_toolbar_new();
    gtk_toolbar_set_style(GTK_TOOLBAR(navbar), GTK_TOOLBAR_ICONS);
    gtk_toolbar_set_icon_size(GTK_TOOLBAR(navbar), GTK_ICON_SIZE_SMALL_TOOLBAR);

GtkToolItem* open = gtk_tool_button_new(NULL, "Open");
gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(open), "document-open");
gtk_toolbar_insert(GTK_TOOLBAR(navbar), open, -1);
g_signal_connect(open, "clicked", G_CALLBACK(on_open_file), NULL);

GtkToolItem* dict = gtk_tool_button_new(NULL, "Dictionaries");
gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(dict), "folder");
gtk_toolbar_insert(GTK_TOOLBAR(navbar), dict, -1);
g_signal_connect(dict, "clicked", G_CALLBACK(on_manage_dictionaries), NULL);

GtkToolItem* settings = gtk_tool_button_new(NULL, "Settings");
gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(settings), "preferences-system");
gtk_toolbar_insert(GTK_TOOLBAR(navbar), settings, -1);

g_signal_connect(settings, "clicked", G_CALLBACK(on_settings), NULL);



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
    GtkWidget *label = gtk_label_new("");
    PangoFontDescription *font_desc = pango_font_description_from_string("Sans 20");
    gtk_widget_override_font(label, font_desc);
    gtk_widget_override_font(entry, font_desc);
    pango_font_description_free(font_desc);

    gtk_box_pack_start(GTK_BOX(hbox), from, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(hbox), to, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), navbar, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 5);
    

    gtk_container_add(GTK_CONTAINER(window), vbox);

    GtkWidget* widgets[4] = {entry, label, from, to};
    
    g_signal_connect(button, "clicked", G_CALLBACK(on_settings), widgets);
    g_signal_connect(button, "clicked", G_CALLBACK(on_manage_dictionaries), widgets);
    g_signal_connect(button, "clicked", G_CALLBACK(on_open_file), widgets);
    g_signal_connect(button, "clicked", G_CALLBACK(on_translate), widgets);
    g_signal_connect(entry, "changed", G_CALLBACK(on_translate), widgets);
    g_signal_connect(from, "changed", G_CALLBACK(on_choose), to);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
