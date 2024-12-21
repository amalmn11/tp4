#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<gtk/gtk.h>
#define MAX 40
#define BL ' '
#define RC '\n'
//definition BI
typedef struct
{
    int xw;//premier terme w:width
    int yh;//deuxieme terme h:height
}Bi;

typedef struct {
    GdkRGBA color;
    gchar hexa_color[30];
} hexa_color;

typedef struct {
    gchar police[50];
    hexa_color color;
    gint taille;
    gint gras;
} Style;



// Définition de la structure représentant un bouton personnalisé
typedef struct
{
    GtkWidget *Button;        // Pointeur vers le widget bouton GTK
    GtkWidget *pere;          // Pointeur vers le widget parent
    gchar text[40];           // Texte affiché sur le bouton
    gchar url_icone[150];     // URL de l'icône associée au bouton
    gint mnemonic;            // Mnémonique associé au bouton
    Bi dim;            // Structure définissant les dimensions du bouton
    //hexa_color back_ground;   // Structure représentant la couleur de fond du bouton
    Bi cord;           // Structure représentant les coordonnées du bouton
}Boutton;

//creation de button
Boutton *creer_button_normale(Boutton *Mb)
{
    // Vérifie si un texte est spécifié
    if (Mb->text[0] != '\0') {
        switch (Mb->mnemonic) {
            // Crée un bouton avec un label
            case 0:
                Mb->Button = gtk_button_new_with_label(Mb->text);
                break;
                // Crée un bouton avec un mnémonique
            case 1:
                Mb->Button = gtk_button_new_with_mnemonic(Mb->text);
                g_signal_connect(Mb->Button, "clicked", G_CALLBACK(gtk_main_quit), NULL);
                break;
        }
    }
        // Vérifie si une URL d'icône est spécifiée
    else if (Mb->url_icone[0] != '\0')
    {
        GtkWidget *button = gtk_button_new();

        // Création d'une image à partir d'un fichier (remplacez "path/to/image.png" par le chemin de votre image)
        GtkWidget *image = gtk_image_new_from_file(Mb->url_icone);

        // Ajout de l'image au bouton
        gtk_button_set_image(GTK_BUTTON(button), image);

        // Assigne le bouton créé à la structure Boutton
        Mb->Button = button;
    }
        // Si ni le texte ni l'icône ne sont spécifiés, crée un bouton vide
    else Mb->Button = gtk_button_new();

    // Définit la taille du bouton en fonction des dimensions spécifiées
    gtk_widget_set_size_request(Mb->Button, Mb->dim.xw, Mb->dim.yh);

    return (Boutton *)Mb;
}



// Fonction appliquer_style_button : applique un style spÃ©cifiÃ© Ã  un bouton
void appliquer_style_button(Style *monStyle, Boutton *Mb)
{
    // CrÃ©ation d'une nouvelle description de police Pango
    PangoFontDescription *font_desc = pango_font_description_new();

    // Appliquer la famille de police spÃ©cifiÃ©e dans le style
    pango_font_description_set_family(font_desc, monStyle->police);

    // Appliquer la taille de police spÃ©cifiÃ©e dans le style
    pango_font_description_set_size(font_desc, monStyle->taille * PANGO_SCALE);

    // Appliquer la police au widget du bouton
    gtk_widget_override_font(Mb->Button, font_desc);

    // LibÃ©rer la mÃ©moire allouÃ©e pour la description de police
    pango_font_description_free(font_desc);

    // Appliquer le style gras si spÃ©cifiÃ© dans le style
    if (monStyle->gras == 1)
    {
        // RÃ©cupÃ©rer le label du bouton
        GtkWidget *label = gtk_bin_get_child(GTK_BIN(Mb->Button));

        // RÃ©cupÃ©rer le texte actuel du label
        const gchar *current_label_text = gtk_label_get_text(GTK_LABEL(label));

        // ConcatÃ©ner le texte avec des balises pour le rendre en gras
        const gchar *bold_text = g_strdup_printf("<b>%s</b>", current_label_text);

        // Appliquer le texte en gras au label
        gtk_label_set_markup(GTK_LABEL(label), bold_text);

        // LibÃ©rer la mÃ©moire allouÃ©e pour le texte en gras
        g_free(bold_text);
    }

    // RÃ©cupÃ©rer le label du bouton
    GtkWidget *lab = gtk_bin_get_child(GTK_BIN(Mb->Button));

    // VÃ©rifier si le widget est un label
    if (GTK_IS_LABEL(lab))
    {
        // Modifier la couleur du texte si spÃ©cifiÃ©e dans le style
        if (monStyle->color.hexa_color[0] != '\0')
        {
            GdkRGBA color;

            // Parse la couleur hexadÃ©cimale et l'applique au texte
            gdk_rgba_parse(&color, monStyle->color.hexa_color);
            gtk_widget_override_color(lab, GTK_STATE_FLAG_NORMAL, &color);
        }

        // Modifier la taille de police si spÃ©cifiÃ©e dans le style
        if (monStyle->taille > 0)
        {
            // Copier la description de police et appliquer la nouvelle taille
            PangoFontDescription *font_desc = pango_font_description_copy(pango_font_description_from_string(monStyle->police));
            gtk_widget_modify_font(lab, font_desc);
            pango_font_description_free(font_desc);
        }
    }
}

// Fonction pour initialiser une couleur hexadÃ©cimale
hexa_color *init_hexa_color(hexa_color *myColor, gchar *hex_code) {
    // VÃ©rifier si la couleur existe dÃ©jÃ 
    if (myColor == NULL) {
        // Allouer de la mÃ©moire pour la couleur si elle n'existe pas
        myColor = malloc(sizeof(hexa_color));
        if (myColor == NULL) {
            fprintf(stderr, "Erreur d'allocation mÃ©moire\n");
            return NULL;
        }
    }
    // Copier le code hexadÃ©cimal dans la structure hexa_color
    g_strlcpy(myColor->hexa_color, hex_code, sizeof(myColor->hexa_color));
    // Convertir le code hexadÃ©cimal en couleur GTK RGBA
    gdk_rgba_parse(&(myColor->color), myColor->hexa_color);
    return (hexa_color *)myColor;
}


///
typedef struct
{
    GtkWidget *window;    // Widget GTK+ de la fenêtre
    // dimension dim;        // Dimensions de la fenêtre
    gint border_size;     // Épaisseur de la bordure de la fenêtre
    gchar title[MAX];     // Titre de la fenêtre (maximum MAX caractères)
    gchar icon_name[MAX]; // Nom de l'icône de la fenêtre
    gint  resisable;      // Indique si la fenêtre est redimensionnable
    // (0: FALSE, 1: TRUE)
    gchar position;       // Position initiale de la fenêtre [c: GTK_WIN_POS_CENTER]
    // ou  [m: GTK_WIN_POS_MOUSE] ...
    //  coordonnees cord;     // Coordonnées de positionnement de la fenêtre
    // par rapport à l'axe y et x
    //  hexa_color bgColor;   // Couleur de fond de la fenêtre
    //  MonImage bgImg;       // Image de fond de la fenêtre
}window;



// DÃ©finition d'une structure MonLabel pour reprÃ©senter un label personnalisÃ© avec des propriÃ©tÃ©s spÃ©cifiques
typedef struct
{
    GtkWidget *Label;    // Pointeur vers le widget du label
    GtkWidget *pere;     // Pointeur vers le widget parent
    gchar *texte;        // Texte du label
    gint mnemonic;       // Indicateur mnÃ©monique
    Bi dim;       // Structure dimension pour stocker la taille du label
    Bi cord;      // Structure cordonnee pour stocker les coordonnÃ©es du label
} Label;


/***********************************************************************************************************
 Nom            : creer_label()
 EntrÃ©e         : Mb - Pointeur vers la structure Label
 Sortie         : Un pointeur vers la structure Label mise Ã  jour avec le label crÃ©Ã©
 Description    : CrÃ©e un label en fonction des paramÃ¨tres spÃ©cifiÃ©s dans la structure Label
***********************************************************************************************************/
// La fonction creer_label crÃ©e un label en fonction des paramÃ¨tres spÃ©cifiÃ©s dans la structure Label.
Label *creer_label(Label *Mb)
{
    // VÃ©rifier si un texte est spÃ©cifiÃ© dans la structure Label
    if (Mb->texte)
    {
        // VÃ©rifier si un mnemonic est spÃ©cifiÃ© dans la structure Label
        if (Mb->mnemonic)
        {
            // CrÃ©er un label avec mnemonic
            Mb->Label = gtk_label_new_with_mnemonic(Mb->texte);
        }
        else
        {
            // CrÃ©er un label sans mnemonic
            Mb->Label = gtk_label_new(Mb->texte);
        }
    }

    // DÃ©finir la taille du label avec les dimensions spÃ©cifiÃ©es dans la structure Label
    gtk_widget_set_size_request(Mb->Label, Mb->dim.xw, Mb->dim.yh);

    // Retourner un pointeur vers la structure Label mise Ã  jour avec le label crÃ©Ã©
    return ((Label *)Mb);
}



GdkPixbuf* create_icon_from_file(const gchar file_path[50])
{
    GdkPixbuf *icon;
    GError *error = NULL;
    icon = gdk_pixbuf_new_from_file(file_path, &error);
    if (error != NULL)
    {
        // Affichage de l'erreur en cas d'échec du chargement de l'icône
        g_printerr("Erreur lors du chargement de l'icône : %s\n", error->message);
        g_error_free(error);
        return NULL;
    }//FIn if (error != NULL)
    return icon;
}//FIN fonction create_icon_from_file()
///////////////////////////////////////////////////////////////////////////////////////////
GtkWidget* create_image_widget_from_file(const gchar *file_path)
{
    GdkPixbuf *pixbuf;
    GtkWidget *image_widget;

    // Charger l'image à partir du fichier spécifié
    pixbuf = gdk_pixbuf_new_from_file(file_path, NULL);
    if (!pixbuf) {
        // Afficher une erreur si le chargement de l'image échoue
        g_printerr("Erreur lors du chargement de l'image : %s\n", file_path);
        return NULL;
    }
    // Créer un widget GtkImage à partir de l'image chargée
    image_widget = gtk_image_new_from_pixbuf(pixbuf);
    // Libérer la mémoire utilisée par le pixbuf, car le widget GtkImage le détient désormais
    g_object_unref(pixbuf);
    return image_widget;
}//FIN fonction create_image_widget_from_file()
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
void set_icon_size(GtkWidget *image_widget, int width, int height)
{
    GdkPixbuf *pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(image_widget));
    if (pixbuf != NULL)
    {
        GdkPixbuf *scaled_pixbuf = gdk_pixbuf_scale_simple(pixbuf, width, height, GDK_INTERP_BILINEAR);
        gtk_image_set_from_pixbuf(GTK_IMAGE(image_widget), scaled_pixbuf);
        g_object_unref(scaled_pixbuf);  // Free the scaled pixbuf after use
    }
}
void ajouterHeader(window *maFenetre,int headerHeight,int headerWidth,char titre[60],char icon[60],int iconWidth,int iconHeight)
{
    GtkWidget *header_bar = gtk_header_bar_new();
    gtk_widget_set_size_request(header_bar, headerWidth, headerHeight);
    gtk_header_bar_set_title(GTK_HEADER_BAR(header_bar), titre);
    gtk_header_bar_set_show_close_button (GTK_HEADER_BAR (header_bar), TRUE);
    // Création d'une icône pour la barre d'en-tête
    GtkWidget *icon_image =create_image_widget_from_file(icon);
    set_icon_size(icon_image,iconWidth,iconHeight);
    gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), icon_image);
    GtkWidget *box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_style_context_add_class (gtk_widget_get_style_context (box), "linked");
    GtkWidget *button1 = gtk_button_new ();
    gtk_container_add (GTK_CONTAINER (button1), gtk_image_new_from_icon_name ("pan-start-symbolic", GTK_ICON_SIZE_BUTTON));
    gtk_container_add (GTK_CONTAINER (box), button1);
    GtkWidget *button2 = gtk_button_new ();
    gtk_container_add (GTK_CONTAINER (button2), gtk_image_new_from_icon_name ("pan-end-symbolic", GTK_ICON_SIZE_BUTTON));
    gtk_container_add (GTK_CONTAINER (box), button2);
    gtk_header_bar_pack_start (GTK_HEADER_BAR (header_bar), box);
    gtk_window_set_titlebar (GTK_WINDOW(maFenetre->window), header_bar);
}//Fin Fonction ajouterHeader()
/////////////
//epeurer blanc de fichier
void Epeurerblanc(FILE *file)
{
    char car;
    while((car=fgetc(file))==BL ||  car== RC);
    ungetc(car,file);

}

//---> fonction qui transforme un caractere en entier
int char_TO_int(char car){
    return ((int)(car-'0'));
}//fin de la fonction char_TO_int

//retourne un indice sur la balise qu'on est sur laquelle
/*

0: objet
1:property
2:child
3:textstyle
4:buttonstyle
5:children
6:signal
*/

int balise(FILE *file)
{
    char motlue[MAX],car;
    int ind;

        ind = 0;
        Epeurerblanc(file);
        while ((car = fgetc(file)) != EOF && (car != BL)  && (car != '>'))
        {
            motlue[ind++] = car; // Lire le mot
        }
        motlue[ind] ='\0';

        printf("\na%sa\n",motlue);
        if(!(strcmp("<window",motlue)))  return 0;
        else if(!(strcmp("<fixed",motlue)))  return 1;
        else if(!(strcmp("<button",motlue)))  return 2;
        else if(!(strcmp("<checkbox",motlue)))  return 3;
        else if(!(strcmp("<radio",motlue)))  return 4;
        else if(!(strcmp("<label",motlue)))  return 5;
        else if(!(strcmp("<TextView",motlue)))  return 6;
        else if(!(strcmp("<property",motlue))) return 22;
        else if(!(strcmp("<child",motlue))) return 23;
        else if(!(strcmp("<textstyle",motlue))) return 24;
        else if(!(strcmp("<buttonstyle",motlue))) return 25;
        else if(!(strcmp("<children",motlue))) return 26;
        else if(!(strcmp("<style",motlue))) return 27;
        else if(!(strcmp("<signal",motlue))) return 28;
        else return -1;

}//fin de la fonction balise


// Fonction pour initialiser un style
Style *init_style(Style *stl, gchar police[50], hexa_color color, gint taille, gint gras) {
    // VÃ©rifier si le style existe dÃ©jÃ 
    if (!stl) {
        // Allouer de la mÃ©moire pour le style s'il n'existe pas
        stl = malloc(sizeof(Style));
        if (!stl) {
            fprintf(stderr, "Erreur d'allocation mÃ©moire\n");
            return NULL;
        }
    }
    // Copier la police dans la structure Style
    g_strlcpy(stl->police, police, sizeof(stl->police));
    // Copier la couleur et les autres propriÃ©tÃ©s dans la structure Style
    stl->color = color;
    stl->taille = taille;
    stl->gras = gras;
    return (Style *)stl;
}



//
void creer_object(FILE *file,GtkWidget *parent);
void window_xml(FILE *file)
{
    gchar *title_gchar,*bgColor_gchar,*icon_gchar;
    char car;
    char resizable,avoirHeader,border_size,coordonne_y,coordonne_x,position_gchar;
    int test, width_int,height_int;
    window* maFenetre =NULL;
    maFenetre=(window*)malloc(sizeof(window));
    // Vérification de l'allocation de mémoire
    if(!maFenetre) exit(-1);
    //si on trouve property balise retourne 22
    while((test=balise(file))==22)
    {
        char mot[MAX];
        int ind=0;
        Epeurerblanc(file);
        fseek(file, 6, SEEK_CUR);
        fscanf(file, "%s", mot);
        printf("%s", mot);
        //recuperation du title
        if (!(strcmp("title\"", mot)))
        {
            char title[MAX];
            Epeurerblanc(file);
            fseek(file, 1, SEEK_CUR);
            while (((car = fgetc(file)) != EOF) && (car != '<'))
            {
                title[ind++] = car; // Lire le mot
            }
            title[ind] = '\0';
            printf("%s\n",title);
            fseek(file, 10, SEEK_CUR);
            title_gchar = g_strdup(title);
            strcpy(maFenetre->title,title_gchar);
        }
            //recuperation de width
        else if (!(strcmp("width\"", mot)))
        {
            char width[MAX];
            Epeurerblanc(file);
            fseek(file, 1, SEEK_CUR);
            while (((car = fgetc(file)) != EOF) && (car != '<'))
            {
                width[ind++] = car; // Lire le mot
            }
            width[ind] = '\0';
            printf("%s\n",width);
            fseek(file, 10, SEEK_CUR);
            //convertir en entier
            width_int=atoi(width);
            printf("widthhhh -> %d",width_int);
        }
            //recuperation de height
        else if (!(strcmp("height\"", mot)))
        {
            char height[MAX];
            Epeurerblanc(file);
            fseek(file, 1, SEEK_CUR);
            while (((car = fgetc(file)) != EOF) && (car != '<'))
            {
                height[ind++] = car; // Lire le mot
            }
            height[ind] = '\0';
            printf("%s\n",height);
            fseek(file, 10, SEEK_CUR);
            //convertir en entier
            height_int=atoi(height);
            printf("heiiiiiight -> %d",height_int);
        }
            //recuperation de resizable
        else if (!(strcmp("resizable\"", mot)))
        {
            Epeurerblanc(file);
            fseek(file, 1, SEEK_CUR);
            while (((car = fgetc(file)) != EOF) && (car != '<'))
            {
                resizable = car; // Lire le mot
            }
            printf("%c\n",resizable);
            fseek(file, 10, SEEK_CUR);
            //convertir en entier
            printf("\nresize -> %d",char_TO_int(resizable));
        }
            //recuperation de border
        else if (!(strcmp("border\"", mot)))
        {
            Epeurerblanc(file);
            fseek(file, 1, SEEK_CUR);
            while (((car = fgetc(file)) != EOF) && (car != '<'))
            {
                border_size = car; // Lire le mot
            }
            printf("%c\n",border_size);
            fseek(file, 10, SEEK_CUR);
            //convertir en entier
            printf("\nborder size -> %d",char_TO_int(border_size));
        }
            //recuperation de position
        else if (!(strcmp("position\"", mot)))
        {
            char position;
            Epeurerblanc(file);
            fseek(file, 1, SEEK_CUR);
            while (((car = fgetc(file)) != EOF) && (car != '<'))
            {
                position= car; // Lire le mot
            }
            printf("\nposition --> %c\n",position);
            fseek(file, 10, SEEK_CUR);
            maFenetre->position=position;
        }
            //recuperation de x
        else if (!(strcmp("x\"", mot)))
        {
            coordonne_x;
            Epeurerblanc(file);
            fseek(file, 1, SEEK_CUR);
            while (((car = fgetc(file)) != EOF) && (car != '<'))
            {
                coordonne_x = car; // Lire le mot
            }
            printf("%c\n",coordonne_x);
            fseek(file, 10, SEEK_CUR);
            //convertir en entier
            printf("\ncoordonne_x -> %d",char_TO_int(coordonne_x));
        }
            //recupearation de y
        else if (!(strcmp("y\"", mot)))
        {
            coordonne_y;
            Epeurerblanc(file);
            fseek(file, 1, SEEK_CUR);
            while (((car = fgetc(file)) != EOF) && (car != '<'))
            {
                coordonne_y = car; // Lire le mot
            }
            fseek(file, 10, SEEK_CUR);
            //convertir en entier
            printf("\ncoordonne_y -> %d",char_TO_int(coordonne_y));
        }
        //recuperation de bgColor
        if (!(strcmp("bgColor\"", mot)))
        {
            char bgColor[MAX];
            Epeurerblanc(file);
            fseek(file, 1, SEEK_CUR);
            while (((car = fgetc(file)) != EOF) && (car != '<'))
            {
                bgColor[ind++] = car; // Lire le mot
            }
            bgColor[ind] = '\0';
            printf("\nbackGround color --> %s\n",bgColor);
            fseek(file, 10, SEEK_CUR);
            bgColor_gchar = g_strdup(bgColor);
            //strcpy(maFenetre->bgColor,bgColor_gchar);
        }
        //recuperation de l'icon
        if (!(strcmp("icon\"", mot)))
        {
            char icon[MAX];
            Epeurerblanc(file);
            fseek(file, 1, SEEK_CUR);
            while (((car = fgetc(file)) != EOF) && (car != '<'))
            {
                icon[ind++] = car; // Lire le mot
            }
            icon[ind] = '\0';
            printf("\nchemin icon --> %s\n",icon);
            fseek(file, 10, SEEK_CUR);
            icon_gchar = g_strdup(icon);
            //strcpy(maFenetre->bgColor,bgColor_gchar);
        }
            //recuperation choix concerant header bar
        else if (!(strcmp("headerBar\"", mot)))
        {
            avoirHeader;
            Epeurerblanc(file);
            fseek(file, 1, SEEK_CUR);
            while (((car = fgetc(file)) != EOF) && (car != '<'))
            {
                avoirHeader = car; // Lire le mot
            }
            fseek(file, 10, SEEK_CUR);
            //convertir en entier
            printf("\navoirHeader -> %d",char_TO_int(avoirHeader));
        }
    }
    maFenetre->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(maFenetre->window), maFenetre->title);
    gtk_window_set_default_size(GTK_WINDOW(maFenetre->window),width_int ,height_int);
    if(maFenetre->position=='c')
        gtk_window_set_position(GTK_WINDOW(maFenetre->window), GTK_WIN_POS_CENTER_ALWAYS);
    else if(maFenetre->position=='m')
        gtk_window_set_position(GTK_WINDOW(maFenetre->window),GTK_WIN_POS_MOUSE);
    gtk_container_set_border_width(GTK_CONTAINER(maFenetre->window), char_TO_int(border_size));
    if(!char_TO_int(resizable))
        gtk_window_set_resizable(GTK_WINDOW(maFenetre->window),FALSE);
    else
        gtk_window_set_resizable(GTK_WINDOW(maFenetre->window),TRUE);
    if(char_TO_int(avoirHeader)==1)
        ajouterHeader(maFenetre,50,900,
                      maFenetre->title,"flower.png",50,50);//remplacer flower par icon_gcar
    //gtk_window_move(GTK_WINDOW(maFenetre->window), coordonne_x,coordonne_y);
    g_signal_connect(maFenetre->window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    //si child balise retourne 23
    if((test==23))
    {
        printf("chilldd???%d",test);
        // creer_fils(file,maFenetre->window);
        creer_object(file,maFenetre->window);
    }
    printf("fin window");
    gtk_widget_show_all(maFenetre->window);

}
void button_xml(FILE *file,GtkWidget *parent)
{   int test,width_int,height_int;
    //
    Bi cord;
     char x[10],y[10];
    gchar *title_gchar,*bgColor_gchar,*icon_gchar;
    char car;
    Boutton *maButton = NULL;
    maButton = (Boutton*)malloc(sizeof(Boutton));
    // Vérification de l'allocation de mémoire
    if(!maButton) exit(-1);
    //si on trouve property balise retourne 22
    while((test=balise(file))==22) {
        char mot[MAX];
        int ind = 0;
        Epeurerblanc(file);
        fseek(file, 6, SEEK_CUR);
        fscanf(file, "%s", mot);
        printf("%s", mot);
        //recuperation du title
        if (!(strcmp("text\"", mot))) {
            char text[MAX];
            Epeurerblanc(file);
            fseek(file, 1, SEEK_CUR);
            while (((car = fgetc(file)) != EOF) && (car != '<')) {
                text[ind++] = car; // Lire le mot
            }
            text[ind] = '\0';
            printf("%s\n", text);
            fseek(file, 10, SEEK_CUR);
            title_gchar = g_strdup(text);
            strcpy(maButton->text, title_gchar);
        }
            //recuperation de l'icon
        else if (!(strcmp("icon\"", mot))) {
            char icon[MAX];
            Epeurerblanc(file);
            fseek(file, 1, SEEK_CUR);
            while (((car = fgetc(file)) != EOF) && (car != '<')) {
                icon[ind++] = car; // Lire le mot
            }
            icon[ind] = '\0';
            printf("\nchemin icon --> %s\n", icon);
            fseek(file, 10, SEEK_CUR);
            icon_gchar = g_strdup(icon);
            g_strlcpy(maButton->url_icone, icon_gchar, sizeof(maButton->url_icone));
        } else if (!(strcmp("mnemonic\"", mot))) {
            char mnemonic;
            Epeurerblanc(file);
            fseek(file, 1, SEEK_CUR);
            while (((car = fgetc(file)) != EOF) && (car != '<')) {
                mnemonic = car; // Lire le mot
            }
            printf("%c\n", mnemonic);
            fseek(file, 10, SEEK_CUR);
            //convertir en entier
            printf("\n mnemonic-> %d", char_TO_int(mnemonic));
            maButton->mnemonic = char_TO_int(mnemonic);
        }
            //recuperation de x
        else if (!(strcmp("x\"", mot))) {
            Epeurerblanc(file);
            fseek(file, 1, SEEK_CUR);
            while (((car = fgetc(file)) != EOF) && (car != '<'))
            {
                x[ind++] = car; // Lire le mot
            }
            x[ind] = '\0';
            printf("%s\n",x);
            fseek(file, 10, SEEK_CUR);
            //convertir en entier
            cord.xw=atoi(x);
            maButton->cord.xw = cord.xw;

        }
            //recupearation de y
        else if (!(strcmp("y\"", mot))) {

            Epeurerblanc(file);
            fseek(file, 1, SEEK_CUR);
            while (((car = fgetc(file)) != EOF) && (car != '<'))
            {
                y[ind++] = car; // Lire le mot
            }
            y[ind] = '\0';
            printf("%s\n",y);
            fseek(file, 10, SEEK_CUR);
            //convertir en entier
            cord.yh=atoi(y);
            maButton->cord.yh = cord.yh;
        }
            //recuperation de width
        else if (!(strcmp("width\"", mot))) {
            char width[MAX];
            Epeurerblanc(file);
            fseek(file, 1, SEEK_CUR);
            while (((car = fgetc(file)) != EOF) && (car != '<')) {
                width[ind++] = car; // Lire le mot
            }
            width[ind] = '\0';
            printf("%s\n", width);
            fseek(file, 10, SEEK_CUR);
            //convertir en entier
            width_int = atoi(width);
            printf("widthhhh -> %d", width_int);
            maButton->dim.xw = width_int;

        }
            //recuperation de height
        else if (!(strcmp("height\"", mot))) {
            char height[MAX];
            Epeurerblanc(file);
            fseek(file, 1, SEEK_CUR);
            while (((car = fgetc(file)) != EOF) && (car != '<')) {
                height[ind++] = car; // Lire le mot
            }
            height[ind] = '\0';
            printf("%s\n", height);
            fseek(file, 10, SEEK_CUR);
            //convertir en entier
            height_int = atoi(height);
            printf("heiiiiiight -> %d", height_int);
            maButton->dim.yh = height_int;
        }


    }
    //gtk_container_add(GTK_CONTAINER(parent),maButton->Button);
    maButton = creer_button_normale(maButton);
    printf("!!!!!!!!!!!!  %d  !!!!!!!!!!!!");
    
    //si le teste est une style
    if(test==27)
    {
        hexa_color *textColor;
        Style *buttonStyle=NULL;
        buttonStyle=(Style*)malloc(sizeof(Style));
        fseek(file, 1, SEEK_CUR);
        Epeurerblanc(file);
        //si on trouve property balise retourne 22
        while((test=balise(file))==22)
        {
            printf("styling");

            char mot[MAX],taille[MAX];
            int ind = 0;
            Epeurerblanc(file);
            fseek(file, 6, SEEK_CUR);
            fscanf(file, "%s", mot);
            printf("%s", mot);
            if (!(strcmp("color\"", mot))) {
                char color[MAX];
                Epeurerblanc(file);
                fseek(file, 1, SEEK_CUR);
                while (((car = fgetc(file)) != EOF) && (car != '<')) {
                    color[ind++] = car; // Lire le mot
                }
                color[ind] = '\0';
                printf("%s\n", color);
                fseek(file, 10, SEEK_CUR);
                buttonStyle->color.hexa_color;
            }
            else if(!(strcmp("police\"", mot)))
            {
                char police[MAX];
                Epeurerblanc(file);
                fseek(file, 1, SEEK_CUR);
                while (((car = fgetc(file)) != EOF) && (car != '<')) {
                    police[ind++] = car; // Lire le mot
                }
                police[ind] = '\0';
                printf("%s\n", police);
                fseek(file, 10, SEEK_CUR);
                strcpy(buttonStyle->police, police);
            }
            else if (!(strcmp("taille\"", mot))) {
                Epeurerblanc(file);
                fseek(file, 1, SEEK_CUR);
                while (((car = fgetc(file)) != EOF) && (car != '<'))
                {
                    taille[ind++] = car; // Lire le mot
                }
                taille[ind] = '\0';
                printf("%s\n",x);
                fseek(file, 10, SEEK_CUR);
                //convertir en entier
                buttonStyle->taille=atoi(taille);
            }else if (!(strcmp("gras\"", mot))) {
                char gras;
                Epeurerblanc(file);
                fseek(file, 1, SEEK_CUR);
                while (((car = fgetc(file)) != EOF) && (car != '<')) {
                    gras = car; // Lire le mot
                }
                printf("%c\n", gras);
                fseek(file, 10, SEEK_CUR);
                //convertir en entier
                buttonStyle->gras= char_TO_int(gras);
            }
        }
        textColor=init_hexa_color(textColor,buttonStyle->color.hexa_color);
        buttonStyle=init_style(buttonStyle, buttonStyle->police, *textColor, buttonStyle->taille, buttonStyle->gras);
        appliquer_style_button(buttonStyle, maButton);
    }
    //si test n'egale a pas propertt (22), ignorer la balise fermante de button
    
    //si test n'egale a pas propertt (22), ignorer la balise fermante de button
    if(test==-1) printf("button fermante");

    
    gtk_fixed_put(GTK_FIXED(parent),maButton->Button,maButton->cord.xw,maButton->cord.yh);
    creer_object(file,parent);
}


void label_xml(FILE *file,GtkWidget *parent)
{
    Bi cord;
    int test,width_int,height_int;
    char x[10],y[10];
    char car;
    gchar *title_gchar,*bgColor_gchar,*icon_gchar;
    Label *label=NULL;
    label=(Label*)malloc(sizeof(Label));
    if(!label) exit(-1);
    //si on trouve property balise retourne 22
    while((test=balise(file))==22)
    {
        g_print("%d??",test);
        char mot[MAX];
        int ind = 0;
        Epeurerblanc(file);
        fseek(file, 6, SEEK_CUR);
        fscanf(file, "%s", mot);
        printf("%s", mot);
        //recuperation du title
        if (!(strcmp("text_label\"", mot))) {
            char text[MAX];
            Epeurerblanc(file);
            //pour eliminer le caractere ">"
            fseek(file, 1, SEEK_CUR);
            while (((car = fgetc(file)) != EOF) && (car != '<')) {
                text[ind++] = car; // Lire le mot
            }
            text[ind] = '\0';
            printf("%s\n", text);
            fseek(file, 10, SEEK_CUR);
            title_gchar = g_strdup(text);
            label->texte = g_strdup(title_gchar);
            printf("%s\n", label->texte);
        }else if (!(strcmp("mnemonic\"", mot))) {
            char mnemonic;
            Epeurerblanc(file);
            fseek(file, 1, SEEK_CUR);
            while (((car = fgetc(file)) != EOF) && (car != '<')) {
                mnemonic = car; // Lire le mot
            }
            printf("%c\n", mnemonic);
            fseek(file, 10, SEEK_CUR);
            //convertir en entier
            printf("\n mnemonic-> %d", char_TO_int(mnemonic));
            label->mnemonic = char_TO_int(mnemonic);
        }
            //recuperation de x
        else if (!(strcmp("x\"", mot))) {
            Epeurerblanc(file);
            fseek(file, 1, SEEK_CUR);
            while (((car = fgetc(file)) != EOF) && (car != '<'))
            {
                x[ind++] = car; // Lire le mot
            }
            x[ind] = '\0';
            printf("%s\n",x);
            fseek(file, 10, SEEK_CUR);
            //convertir en entier
            cord.xw=atoi(x);
            label->cord.xw = cord.xw;

        }
            //recupearation de y
        else if (!(strcmp("y\"", mot))) {

            Epeurerblanc(file);
            fseek(file, 1, SEEK_CUR);
            while (((car = fgetc(file)) != EOF) && (car != '<'))
            {
                y[ind++] = car; // Lire le mot
            }
            y[ind] = '\0';
            printf("%s\n",y);
            fseek(file, 10, SEEK_CUR);
            //convertir en entier
            cord.yh=atoi(y);
            label->cord.yh = cord.yh;
        }
            //recuperation de width
        else if (!(strcmp("width\"", mot))) {
            char width[MAX];
            Epeurerblanc(file);
            fseek(file, 1, SEEK_CUR);
            while (((car = fgetc(file)) != EOF) && (car != '<')) {
                width[ind++] = car; // Lire le mot
            }
            width[ind] = '\0';
            printf("%s\n", width);
            fseek(file, 10, SEEK_CUR);
            //convertir en entier
            width_int = atoi(width);
            printf("widthhhh -> %d", width_int);
            label->dim.xw = width_int;

        }
            //recuperation de height
        else if (!(strcmp("height\"", mot))) {
            char height[MAX];
            Epeurerblanc(file);
            fseek(file, 1, SEEK_CUR);
            while (((car = fgetc(file)) != EOF) && (car != '<')) {
                height[ind++] = car; // Lire le mot
            }
            height[ind] = '\0';
            printf("%s\n", height);
            fseek(file, 10, SEEK_CUR);
            //convertir en entier
            height_int = atoi(height);
            printf("heiiiiiight -> %d", height_int);
            label->dim.yh = height_int;
        }
    }
    if(test==-1) printf("label fermante");

    //
    label = creer_label(label);
    gtk_fixed_put(GTK_FIXED(parent),label->Label,label->cord.xw,label->cord.yh);
    //gtk_container_add(GTK_CONTAINER(parent),maButton->Button);
    creer_object(file,parent);
}

void creer_object(FILE *file,GtkWidget *parent)
{
    //id est l'id de widget d'un objet : window , label , box etc
    int id = balise(file);
    g_print("***%d***\n",id);
    switch(id)
    {
        case 0: window_xml(file) ;break;
        case 1:{
            int test;
            GtkWidget *fixed = gtk_fixed_new();
            gtk_container_add(GTK_CONTAINER(parent),fixed);
            if((test=balise(file))==26)
            {
                printf("testtttt::::%d",test);

                creer_object(file,fixed);
            }
            break;
        }
        case 2: button_xml(file,parent);break;
        case 3:checkbox_xml(file,parent); break;
        case 4:radio_xml(file,parent);break;
        case 5:label_xml(file,parent);break;
        case 6:TextView_xml(file,parent);break;
        default:printf("pas de children");break;
    }
}



int main(int argc, char *argv[]) {

    // Initialisation de GTK
    gtk_init(&argc, &argv);

    FILE *file = fopen("xml.txt","r");
    if (!file)
    {
        printf("ERREUR d'ouverture du fichier !!");
        return -1;
    }
    GtkWidget *parent =NULL;
    creer_object(file,parent);
    gtk_main();
    return 0;
}
