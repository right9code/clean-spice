/*
 * clean-spice: Minimal, zero-header, frameless SPICE client for Wayland tiling window managers.
 * Author: right9code
 * License: Non-Commercial Use Only (Personal & Educational Use Only. Commercial Use Strictly Prohibited).
 */

#include <gtk/gtk.h>
#include <spice-client-gtk.h>
#include <stdio.h>

static void on_channel_new(SpiceSession *session, SpiceChannel *channel, gpointer user_data) {
    GtkWindow *window = GTK_WINDOW(user_data);

    if (SPICE_IS_DISPLAY_CHANNEL(channel)) {
        int channel_id = 0;
        g_object_get(channel, "channel-id", &channel_id, NULL);

        if (channel_id == 0) {
            SpiceDisplay *display = spice_display_new(session, channel_id);
            g_object_set(display,
                         "scaling", TRUE,
                         "resize-guest", TRUE,
                         NULL);

            gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(display));
            gtk_widget_show_all(GTK_WIDGET(window));
        }
    }
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    const char *uri = (argc > 1) ? argv[1] : "spice://127.0.0.1:5900";

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Windows 11");
    gtk_window_set_default_size(GTK_WINDOW(window), 1280, 720);

    /* Strip all CSD headerbars, menus, and client window decorations */
    gtk_window_set_decorated(GTK_WINDOW(window), FALSE);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    SpiceSession *session = spice_session_new();
    g_object_set(session, "uri", uri, NULL);

    /* Enable seamless bidirectional clipboard and USB auto-redirection */
    SpiceGtkSession *gtk_session = spice_gtk_session_get(session);
    g_object_set(gtk_session,
                 "auto-clipboard", TRUE,
                 "auto-usbredir", TRUE,
                 NULL);

    g_signal_connect(session, "channel-new", G_CALLBACK(on_channel_new), window);

    if (!spice_session_connect(session)) {
        fprintf(stderr, "clean-spice: Failed to connect to SPICE server at %s\n", uri);
        return 1;
    }

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
