 Initiation à la SDL 2

## Niveau: 1 A

## Plan

#### 1. Introduction

#### 2. Initialisation de la SDL 2

#### 3. Création d’une fenêtre

#### 4. SDL_Renderer

#### 5. Afficher une image

#### 6. Afficher un texte

#### 7. Son continu

#### 8. Son bref

#### 9. Evénements

#### 10. Conclusion

## Introduction

Dans cette présentation, nous allons découvrir **SDL 2** (Simple DirectMedia Layer), une bibliothèque en
langage C utilisée pour le développement d’applications graphiques et multimédias telles que les jeux vidéo.
**Pourquoi SDL 2?**
SDL 2 permet de :
ü Créer des fenêtres graphiques
ü Gérer le rendu 2 D (images, textures)
ü Afficher du texte
ü Gérer le son (continu et bref)
ü Traiter les événements (clavier, souris, fermeture)

## Initialisation de la SDL 2

```
ü SDL 2 est composée de plusieurs sous-systèmes tels que : vidéo , audio, gestion du temps,
gestion des événements
ü L’initialisation de SDL 2 se fait à l’aide de la fonction SDL_Init().
ü Il est possible d’initialiser uniquement les sous-systèmes nécessaires en précisant leurs noms.
```

```
ü SDL_Quit(): permet de libérer les ressources utilisées par les sous systèmes SDL déjà initialisés.
```

```
SDL_Init (SDL_INIT_VIDEO | SDL_INIT_AUDIO) ;
```

```
ü La fonction SDL_Init() retourne 0 si l’initialisation s’est déroulée avec succès.
```

## Création d’une fenêtre

ü En SDL 2 , la création d’une fenêtre graphique se fait à l’aide de la fonction **SDL_CreateWindow().**
ü La gestion de l’affichage est séparé en deux éléments :
Ø **SDL_Window** : représente la fenêtre
Ø **SDL_Renderer** : gère le rendu graphique **SDL_Window* SDL_CreateWindow(
cinotn xs,t char* title,
iinntt yw,,
iUnitn ht 3 , 2 flags
);**

## Création d’une fenêtre

#### Les paramètres de SDL_CreateWindow ()

#### ü title : titre de la fenêtre

#### ü x, y : position de la fenêtre à l’écran

#### (SDL_WINDOWPOS_CENTERED pour centrer)

#### ü w, h : largeur et hauteur de la fenêtre (en pixels)

#### ü flags : options de création de la fenêtre

## Création d’une fenêtre

#### Flags courants

ü **SDL_WINDOW_SHOWN** : fenêtre visible
ü **SDL_WINDOW_RESIZABLE** : fenêtre redimensionnable
ü **SDL_WINDOW_FULLSCREEN** : plein écran
ü **SDL_WINDOW_BORDERLESS** : sans bordure

- -> Si aucun flag n’est nécessaire, on peut utiliser 0

#### Exemple

SDL_Window *window = NULL;
window = SDL_CreateWindow( "Ma première fenêtre SDL 2 ", SDL_WINDOWPOS_CENTERED,
SDL_WINDOWPOS_CENTERED, 800 , 600 , Z. F 0 );^7

## SDL_Renderer – Gestion du rendu graphique dans SDL 2

```
Qu’est-ce que SDL_Renderer?
ü SDL_Renderer est une structure de SDL 2 qui permet de dessiner des images, formes et textures sur la
fenêtre.
ü Il sert d’interface entre le code et le GPU.
ü Utilisé pour le rendu 2 D dans les applications graphiques ou jeux.
Création d’un Renderer
SDL_Renderer* SDL_CreateRenderer(
SDL_Window* window,
int index,
Uint 32 flags
);
```

## SDL_Renderer – Gestion du rendu graphique dans SDL 2

#### Les paramètres

```
ü window : pointeur vers la SDL_Window créée
ü index : numéro du driver ; - 1 signifie utiliser le driver par défaut
ü flags : options du rendu ( renderer ), 0 pour la valeur par défaut (pas d’accélération GPU ni vsync)
```

#### Exemple de flags

```
ü SDL_RENDERER_SOFTWARE : Utilise le rendu logiciel (CPU), non pas GPU.
ü SDL_RENDERER_ACCELERATED : Utilise le GPU pour accélérer le rendu.
ü SDL_RENDERER_PRESENTVSYNC : Synchronise le rendu avec le rafraîchissement de l’écran
(évite le tearing).
```

## SDL_Renderer – Gestion du rendu graphique dans SDL 2

#### Les Fonctions principales

```
ü SDL_SetRenderDrawColor(renderer, r, g, b, a) : Définit la couleur pour le dessin
ü SDL_RenderClear(renderer) : Efface l’écran avec la couleur définie
ü SDL_RenderDrawRect(renderer, &rect) : dessine un rectangle
ü SDL_RenderFillRect(renderer, &rect) : remplit un rectangle
ü SDL_RenderCopy(renderer, texture, &src, &dest) : copie une texture sur le rendu
ü SDL_RenderPresent(renderer) : affiche le rendu à l’écran
```

## Afficher une image

(^1) P**.** o (^) u **B** r **i** c **b** h **li** a **o** rg **th** er **è** e **q** t **u** a **e** ff **s** i (^) cher une image avec SDL 2 :

## iinncclluuddee <<SSDDLL 22 //SSDDLL._hi>m (^) a (^) g (^) e (^) .h>

(^2) S**.** D **I** L **n**_**i** I **t** n **ia** it **l** ( **i** S **s** D **at** L **io**_**n** INIT_VIDEO);
IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
**3. Création de la fenêtre et du renderer**
(^4) S**.** D **C** L **h**_T **a** e **r** x **g** t **e** u **m** re* **e n** im **t** (^) a **d** g **e** e (^) **l** = **’i mIMaGge_LoadTexture** (renderer, "mon_image.png");
i f (p!irminatfg(e")E {rreur chargement image: %s\n", IMG_GetError());
} Z.F 11

## Afficher une image

**5. Affichage de l’image**
ü SDL_RenderClear(renderer); // Efface l’écran
ü SDL_RenderCopy(renderer, image, NULL, NULL); // Dessine l’image entière
ü SDL_RenderPresent(renderer); // Met à jour l’écran

## Le type SDL_Rect

#### ü Le type SDL_Rect définit une zone rectangulaire

#### ü Il s’agit d’une structure prédéfinie dans la bibliothèque SDL.h. Cette structure est composée des champs

```
suivants:
```

- x: l'abscisse
- y: l’ordonnée
- w: la largeur
- h: l’hauteur

#### ü srcrect : la valeur de ce champ vaut

- NULL: si vous souhaitez afficher la totalité de l’image
- adresse d’une variable de type SDL_Rect: si vous souhaitez afficher une partie de l’image.

#### ü dstrect: la valeur de ce champ vaut l’adresse d’une position pos déclarée et initialisée précédemment

## Afficher un texte

**1. Bibliothèque à inclure**

# include <SDL 2 /SDL.h>

# include <SDL 2 /SDL_ttf.h> // Pour gérer le texte

**2. Initialisation**
TTF_Init(); // Initialisation de SDL_ttf
**3. Charger une police**
TTF_Font* police = TTF_OpenFont("chemin/vers/police.ttf", taille);

#### ü chemin/vers/police.ttf : fichier de police (.ttf)

#### ü taille : taille du texte

#### ü Sur Ubuntu : /usr/share/fonts/truetype/ contient déjà plusieurs polices

## Afficher un texte

**4. Définir la couleur**
SDL_Color couleurNoir = { 0 , 0 , 0 , 255 }; // RGBA

ü Exemple : noir → { 0 , 0 , 0 }

**5. Créer la texture du texte**
ü SDL_Surface*surfaceTexte = TTF_RenderText_Blended(police, "Message", couleurNoir);
ü SDL_Texture* textureTexte = SDL_CreateTextureFromSurface(renderer, surfaceTexte);
ü SDL_FreeSurface(surfaceTexte); // plus besoin de la surface

## Afficher un texte

**6. Afficher la texture**
ü SDL_Rect dst = {x, y, largeur, hauteur};
ü SDL_RenderCopy(renderer, textureTexte, NULL, &dst);
ü SDL_RenderPresent(renderer); // Affichage à l’écran

## Son continu

(^1) #**.** i (^) n **B** c **i** l **b** u **l** d **i** e **o t** < **h** S **è** D **qu** L **e** 2 **s** / (^) S **à** D **in** L **c** .h **lu** > **re**

# include <SDL 2 /SDL_mixer.h> // Gestion audio

(^2) if**.** (^) ( **I** M **ni** i **t** x **ia**_**l** O **is** p **a** e **t** n **io** A **n** u **d** d **e** io **S** ( 4 **D** 4 **L** 10 **_** 0 **m** , **i** M **xe** I **r** X_DEFAULT_FORMAT, 2 , 2048 ) < 0 ) {
(^) } printf("Erreur Mix_OpenAudio : %s\n", Mix_GetError());
(^3) M**.** (^) i **C** x_**h** M **ar** u **g** s **e** i **m** c* **e** m **nt** u **d** si **e** q **l** u **a** e (^) **m** = **u** M **si** i **q** x **u**_**e** LoadMUS("musique.mp 3 ");
i f (p!mrinutsfi(q"uEer)r (^) e{ur Mix_LoadMUS: %s\n", Mix_GetError());
}
(^4) M**.** (^) i **J** x **o**_ **u** P **e** l **r** a (^) y **la** M **m** us **u** ic **si** ( **q** m **u** u **e** sique, - 1 ); // - 1 = boucle infinie

## Son continu

**5. Arrêt et libération**

MMiixx__HFraeletMMuussiicc(()m; (^) u (^) s (^) i (^) q (^) u (^) e (^) ) (^) ; (^) //// ALirbrêétreerr llaa mmuussiiqquuee
MSDixL__CQlousite(A); u d i o ( ) ; / (^) // /Q Fueitrtmere rS SDDLL 2 _mixer

## Son continu ( Exemple )

## iinncclluuddee <<SSDDLL 22 //SSDDLL._hm>ixer.h>

i n tS mDaLin_(I)n {it(SDL_INIT_AUDIO);
Mix_OpenAudio( 44100 , MIX_DEFAULT_FORMAT, 2 , 2048 );
Mif (ix!m_Musuiqsiuce*) mreutusirqnu 1 e; = Mix_LoadMUS("musique.mp 3 ");

Mix_PlayMusic(musique, - 1 ); // boucle infinie
SDL_Delay( 10000 ); // laisse la musique jouer 10 secondes
MMiixx__HFraeletMMuussiicc(()m;usique);
MSDixL__CQlousite(A);udio();

(^) } return 0 ;

## Son bref

(^1) #**.** i (^) n **B** c **i** l **b** u **l** d **i** e **o t** < **h** S **è** D **qu** L **e** 2 **s** /SDL.h>

# include <SDL 2 /SDL_mixer.h>

(^2) S**.** D **I** L **n**_**it** I **i** n **a** i **l** t **i** ( **s** S **a** D **tio** L **n** *INIT_AUDIO); // Initialiser SDL audio
Mix_OpenAudio( 44100 , MIX_DEFAULT_FORMAT, 2 , 2048 ); // fréquence, format, stéréo, tampon
(^3) M**.** (^) i **C** x* **h** C **ar** h **g** u **e** n **r** k **l** * **e** (^) s **s** o **o** n **n** (^) = **b** M **ref** ix_LoadWAV("son.wav");
i f (p!srionnt)f (^) ({"Erreur chargement son : %s\n", Mix_GetError());
}
ü Mix_Chunk → pour effets courts (clic, tir, saut...)

## Son bref

(^4) M**.** (^) i **J** x **o** ***u** P **e** l **r** a (^) y **le** C **s** h **o** a **n** n (^) nel(- 1 , son, 0 ); // - 1 = premier canal libre, 0 = joue 1 fois
(^5) M**.** (^) i **A** x **r*** **r** H **ê** a **t** (^) l **(** t **o** C **p** h **t** a **i** n **o** n **n** e **n** l **e** (- **l)** 1 ); // Arrête tous les sons sur tous les canaux
(^6) M**.** (^) i **L** x **i** _**b** F **é** r **r** e **a** e **t** C **io** h **n** u (^) n **d** k **e** ( **s** s (^) o **re** n **s** ) **s** ; **o u r c e s** // Libérer le son
MSDixL__CQlousite(A); u d i o ( ) ; / /// QFueirtmteerr S SDDLL_mixer

##### 22

#### Un évènement: un « signal » envoyé par un périphérique à votre application

#### ü Appuie sur une touche du clavier

#### ü Clic avec la souris

#### ü Bouger la souris

#### ü Réduire/ fermer la fenêtre

.....

## Les événements en SDL

```
Z.F
```

##### 23

#### Pour lire un événement, vous devez

#### 1. déclarer une variable de type SDL_Event

#### 2. Lire l'événement avec une de ces de fonctions

#### ü SDL_PollEvent: lit l'évènement sans bloquer la boucle du jeu

#### ü SDL_WaitEvent: reste en attente jusqu’à écouter un évènement

#### Remarque: si vous avez des traitements automatiques qui ne dépendent pas de

#### l’utilisateur tels que une animation de background, un déplacement d’ennemis..., vous

#### avez intérêt às utiliser la première fonction: SDL_PollEvent

## Les événements en SDL

```
Z.F
```

##### 24

#### ü Soit le code suivant permettant de lire un évènement

#### SDL_Event event

#### SDL_PollEvent(&event); // a appeler dans la boucle du jeu

#### ü Pour détecter un appui sur une touche clavier, il suffit de comparer le type de

#### l'événement avec

#### if( event.type== SDL_KEYDOWN )

#### ü Pour savoir qu’elle touche a été appuyée, vous devez faire cette comparaison

#### if(event.key.keysym.sym == nom_de_la touche)

#### Exemple : if(event.key.keysym.sym ==SDLK_ESCAPE)

## Les événements en SDL

```
Z.F
```

## SDL KEY CODE

##### 26

#### ü Pour détecter un mouvement de la souris, il suffit de comparer le type de l'événement

#### avec

#### if( event.type== SDL_MOUSEMOTION )

#### ü Pour détecter un clic de la souris, il suffit de comparer le type de l'événement avec

#### if( event.type==S DL_MOUSEBUTTONUP ) //ou SDL_MOUSEBUTTONDOWN

#### ü Pour savoir ou le mouvement/clic de la souris a eu lieu, il faut tester si la position de la

#### souris est au dessus de la surface désirée de coordonnée (X,Y,W,H)

#### if(event.motion.x> =X && event.motion.x<=X+W &&

#### event.motion.y>=Y&& event.motion.y<=Y+H)

#### }

## Les événements en SDL

```
Z.F
```

##### 27

```
Déclarer une variable de type SDL_Event
```

**LReecntvuoreie d 1 e ss'** (^) **iél vyé an eumn eénvétsnement
0 sinon**

## Les événements en SDL

**Exemple:**

```
Z.F
```

##### 28

#### ü Détruire la fenêtre : SDL_DestroyWindow(window)

#### ü Détruire le renderer : SDL_DestroyRenderer(renderer)

#### ü Libération de la SDL : SDL_Quit()

#### ü Libération de la TTF : TTF_Quit()

#### ü Libération de la SDL_image : IMG_Quit()

#### ü Libération du son : Mix_CloseAudio(); Mix_Quit()

#### ü Libération de l’espace de la texture : SDL_DestroyTexture(texture)

#### ü Libération d’une Surface : SDL_FreeSurface(surface)

## Libération de l’espace mémoire

```
Z.F
```

## Conclusion

### ü SDL est une bibliothèque qui permet la création d’applications

### graphiques

### ü Lors de la compilation d’un code faisant appel aux fonctions de la

### bibliothèque SDL, vous devez ajouter les options suivantes dans

### l’étape d’édition des liens

### Ø -lSDL 2

### Ø -lSDL 2 _ttf

### Ø -lSDL 2 _image

### Ø -lSDL 2 _mixer
