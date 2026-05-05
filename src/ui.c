#include "calc.h"

static char current_expr[MAX_EXPR_LEN] = "";
static Token rpn_buffer[MAX_EXPR_LEN];

static void update_display(void) {
    gtk_entry_set_text(GTK_ENTRY(display), current_expr);
}

static void append_char(char c) {
    size_t len = strlen(current_expr);
    if (len < MAX_EXPR_LEN - 1) {
        current_expr[len] = c;
        current_expr[len + 1] = '\0';
        update_display();
    }
}

static void clear_display(void) {
    current_expr[0] = '\0';
    update_display();
}

static void calculate(void) {
    if (strlen(current_expr) == 0) return;
    
    int token_count = parse_expr(current_expr, rpn_buffer, MAX_EXPR_LEN);
    if (token_count <= 0) {
        gtk_entry_set_text(GTK_ENTRY(display), "ERR");
        return;
    }
    
    int error;
    double result = eval_rpn(rpn_buffer, token_count, &error);
    
    if (error == 2) {
        gtk_entry_set_text(GTK_ENTRY(display), "DBZ");
        return;
    }
    if (error) {
        gtk_entry_set_text(GTK_ENTRY(display), "ERR");
        return;
    }
    
    char result_str[64];
    if (result == (double)(long long)result) {
        snprintf(result_str, sizeof(result_str), "%.0f", result);
    } else {
        snprintf(result_str, sizeof(result_str), "%.10g", result);
    }
    
    strncpy(current_expr, result_str, MAX_EXPR_LEN - 1);
    current_expr[MAX_EXPR_LEN - 1] = '\0';
    update_display();
}

static void on_num_clicked(GtkButton *button, gpointer data) {
    const char *label = gtk_button_get_label(button);
    append_char(label[0]);
}

static void on_op_clicked(GtkButton *button, gpointer data) {
    const char *label = gtk_button_get_label(button);
    append_char(label[0]);
}

static void on_eq_clicked(GtkButton *button, gpointer data) {
    calculate();
}

static void on_clear_clicked(GtkButton *button, gpointer data) {
    clear_display();
}

GtkWidget* create_ui(void) {
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "TinyCalc");
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_set_border_width(GTK_CONTAINER(main_box), 8);
    
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
    
    display = gtk_entry_new();
    gtk_editable_set_editable(GTK_EDITABLE(display), FALSE);
    gtk_widget_set_size_request(display, -1, 35);
    gtk_grid_attach(GTK_GRID(grid), display, 0, 0, 4, 1);
    
    const char *buttons[4][4] = {
        {"7", "8", "9", "/"},
        {"4", "5", "6", "*"},
        {"1", "2", "3", "-"},
        {"0", "C", "=", "+"}
    };
    
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            GtkWidget *btn = gtk_button_new_with_label(buttons[row][col]);
            gtk_widget_set_size_request(btn, 50, 45);
            gtk_grid_attach(GTK_GRID(grid), btn, col, row + 1, 1, 1);
            
            if (buttons[row][col][0] >= '0' && buttons[row][col][0] <= '9') {
                g_signal_connect(btn, "clicked", G_CALLBACK(on_num_clicked), NULL);
            } else if (buttons[row][col][0] == 'C') {
                g_signal_connect(btn, "clicked", G_CALLBACK(on_clear_clicked), NULL);
            } else if (buttons[row][col][0] == '=') {
                g_signal_connect(btn, "clicked", G_CALLBACK(on_eq_clicked), NULL);
            } else {
                g_signal_connect(btn, "clicked", G_CALLBACK(on_op_clicked), NULL);
            }
        }
    }
    
    gtk_box_pack_start(GTK_BOX(main_box), grid, FALSE, FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window), main_box);
    
    gtk_widget_show_all(window);
    
    return window;
}