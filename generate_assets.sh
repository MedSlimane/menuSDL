#!/usr/bin/env bash
# generate_assets.sh – Create placeholder PNG assets for all 6 menus
# Uses ImageMagick (convert / magick).  Run from the project root:
#     chmod +x generate_assets.sh && ./generate_assets.sh
set -euo pipefail

# Detect ImageMagick command
if command -v magick &>/dev/null; then
    CONVERT="magick"
elif command -v convert &>/dev/null; then
    CONVERT="convert"
else
    echo "ERROR: ImageMagick not found. Install it first."
    exit 1
fi

# ── Helper: make a solid‑colour button with centred text ──────
# btn <file> <WxH> <bg_colour> <text_colour> <label>
btn() {
    local file="$1" size="$2" bg="$3" fg="$4" label="$5"
    $CONVERT -size "$size" "xc:$bg" \
        -fill "$fg" -gravity center -pointsize 30 -annotate +0+0 "$label" \
        -bordercolor black -border 2 \
        "$file"
}

# ── Helper: hover variant (lighter) ──────────────────────────
btn_h() {
    local file="$1" size="$2" bg="$3" fg="$4" label="$5"
    $CONVERT -size "$size" "xc:$bg" \
        -fill "$fg" -gravity center -pointsize 30 -annotate +0+0 "$label" \
        -bordercolor "#444444" -border 3 \
        "$file"
}

# ── Helper: background image ────────────────────────────────
bg() {
    local file="$1" colour="$2" label="$3"
    $CONVERT -size 1920x1080 "xc:$colour" \
        -fill white -gravity NorthEast -pointsize 24 -annotate +20+20 "$label" \
        "$file"
}

# ── Helper: star image with number ──────────────────────────
star() {
    local file="$1" num="$2"
    $CONVERT -size 128x128 xc:transparent \
        -fill gold -stroke black -strokewidth 2 \
        -draw "polygon 64,5 75,45 115,45 82,70 93,110 64,85 35,110 46,70 13,45 53,45" \
        -fill black -stroke none -gravity center -pointsize 36 -annotate +0+10 "$num" \
        "$file"
}

echo "=== Generating placeholder assets ==="

# ────────────────────────────────────────────────────────────
# 1. MAIN MENU
# ────────────────────────────────────────────────────────────
DIR="mainmenu/assets"; mkdir -p "$DIR"
bg   "$DIR/background1.png" "#2c3e50"  "arriere-plan 1"

# Logo placeholder (hexagon‑ish on transparent)
$CONVERT -size 400x300 xc:transparent \
    -fill "#3498db" -stroke black -strokewidth 3 \
    -draw "polygon 200,10 370,80 370,220 200,290 30,220 30,80" \
    -fill white -gravity center -pointsize 40 -annotate +0+0 "Logo" \
    "$DIR/logo.png"

btn   "$DIR/btn_jouer.png"      400x100  "#ecf0f1" black "Jouer"
btn_h "$DIR/btn_jouer_h.png"    400x100  "#bdc3c7" black "Jouer"
btn   "$DIR/btn_options.png"    400x100  "#ecf0f1" black "Options"
btn_h "$DIR/btn_options_h.png"  400x100  "#bdc3c7" black "Options"
btn   "$DIR/btn_scores.png"     400x100  "#ecf0f1" black "Meilleurs Scores"
btn_h "$DIR/btn_scores_h.png"   400x100  "#bdc3c7" black "Meilleurs Scores"
btn   "$DIR/btn_histoire.png"   400x100  "#ecf0f1" black "Histoire"
btn_h "$DIR/btn_histoire_h.png" 400x100  "#bdc3c7" black "Histoire"
btn   "$DIR/btn_quitter.png"    200x80   "#e74c3c" white "Quitter"
btn_h "$DIR/btn_quitter_h.png"  200x80   "#c0392b" white "Quitter"

echo "  [OK] mainmenu/assets"

# ────────────────────────────────────────────────────────────
# 2. OPTION MENU
# ────────────────────────────────────────────────────────────
DIR="optionmenu/assets"; mkdir -p "$DIR"
bg   "$DIR/background2.png" "#34495e" "arriere-plan 2"

btn   "$DIR/btn_diminuer.png"     200x80  "#ecf0f1" black "Diminuer"
btn_h "$DIR/btn_diminuer_h.png"   200x80  "#bdc3c7" black "Diminuer"
btn   "$DIR/btn_augmenter.png"    200x80  "#ecf0f1" black "Augmenter"
btn_h "$DIR/btn_augmenter_h.png"  200x80  "#bdc3c7" black "Augmenter"
btn   "$DIR/btn_normal.png"       200x80  "#ecf0f1" black "Normal"
btn_h "$DIR/btn_normal_h.png"     200x80  "#bdc3c7" black "Normal"
btn   "$DIR/btn_pleinecran.png"   200x80  "#ecf0f1" black "Plein ecran"
btn_h "$DIR/btn_pleinecran_h.png" 200x80  "#bdc3c7" black "Plein ecran"
btn   "$DIR/btn_retour.png"       200x80  "#95a5a6" black "Retour"
btn_h "$DIR/btn_retour_h.png"     200x80  "#7f8c8d" black "Retour"

echo "  [OK] optionmenu/assets"

# ────────────────────────────────────────────────────────────
# 3. SAVE MENU
# ────────────────────────────────────────────────────────────
DIR="savemenu/assets"; mkdir -p "$DIR"
bg   "$DIR/background2.png" "#34495e" "arriere-plan 2"

btn   "$DIR/btn_oui.png"       200x80  "#27ae60" white "OUI"
btn_h "$DIR/btn_oui_h.png"    200x80  "#2ecc71" white "OUI"
btn   "$DIR/btn_non.png"       200x80  "#e74c3c" white "NON"
btn_h "$DIR/btn_non_h.png"    200x80  "#c0392b" white "NON"
btn   "$DIR/btn_charger.png"   400x100  "#ecf0f1" black "Charger le jeu"
btn_h "$DIR/btn_charger_h.png" 400x100  "#bdc3c7" black "Charger le jeu"
btn   "$DIR/btn_nouvelle.png"  400x100  "#ecf0f1" black "Nouvelle Partie"
btn_h "$DIR/btn_nouvelle_h.png" 400x100  "#bdc3c7" black "Nouvelle Partie"

echo "  [OK] savemenu/assets"

# ────────────────────────────────────────────────────────────
# 4. PLAYER MENU
# ────────────────────────────────────────────────────────────
DIR="playermenu/assets"; mkdir -p "$DIR"
bg   "$DIR/background2.png" "#34495e" "arriere-plan 2"

btn   "$DIR/btn_mono.png"       400x100  "#ecf0f1" black "Mono joueur"
btn_h "$DIR/btn_mono_h.png"    400x100  "#bdc3c7" black "Mono joueur"
btn   "$DIR/btn_multi.png"      400x100  "#ecf0f1" black "Multi joueurs"
btn_h "$DIR/btn_multi_h.png"   400x100  "#bdc3c7" black "Multi joueurs"
btn   "$DIR/btn_avatar1.png"    300x120  "#3498db" white "Avatar1"
btn_h "$DIR/btn_avatar1_h.png" 300x120  "#2980b9" white "Avatar1"
btn   "$DIR/btn_avatar2.png"    300x120  "#3498db" white "Avatar2"
btn_h "$DIR/btn_avatar2_h.png" 300x120  "#2980b9" white "Avatar2"
btn   "$DIR/btn_input1.png"     300x100  "#e67e22" white "Input1"
btn_h "$DIR/btn_input1_h.png"  300x100  "#d35400" white "Input1"
btn   "$DIR/btn_input2.png"     300x100  "#e67e22" white "Input2"
btn_h "$DIR/btn_input2_h.png"  300x100  "#d35400" white "Input2"
btn   "$DIR/btn_valider.png"    300x100  "#27ae60" white "Valider"
btn_h "$DIR/btn_valider_h.png" 300x100  "#2ecc71" white "Valider"
btn   "$DIR/btn_retour.png"     200x80   "#95a5a6" black "Retour"
btn_h "$DIR/btn_retour_h.png"  200x80   "#7f8c8d" black "Retour"

echo "  [OK] playermenu/assets"

# ────────────────────────────────────────────────────────────
# 5. SCORE MENU
# ────────────────────────────────────────────────────────────
DIR="scoremenu/assets"; mkdir -p "$DIR"
bg   "$DIR/background3.png" "#1a1a2e" "arriere-plan 3"

btn   "$DIR/btn_valider.png"   200x80  "#27ae60" white "Valider"
btn_h "$DIR/btn_valider_h.png" 200x80  "#2ecc71" white "Valider"
btn   "$DIR/btn_quitter.png"   200x80  "#e74c3c" white "Quitter"
btn_h "$DIR/btn_quitter_h.png" 200x80  "#c0392b" white "Quitter"
btn   "$DIR/btn_retour.png"    200x80  "#95a5a6" black "Retour"
btn_h "$DIR/btn_retour_h.png"  200x80  "#7f8c8d" black "Retour"

star "$DIR/star1.png" "1"
star "$DIR/star2.png" "2"
star "$DIR/star3.png" "3"

echo "  [OK] scoremenu/assets"

# ────────────────────────────────────────────────────────────
# 6. ENIGME MENU
# ────────────────────────────────────────────────────────────
DIR="engimemenu/assets"; mkdir -p "$DIR"
bg   "$DIR/background4.png" "#0d0d0d" "arriere-plan 4"

btn   "$DIR/btn_quiz.png"      300x120  "#8e44ad" white "QUIZ"
btn_h "$DIR/btn_quiz_h.png"   300x120  "#9b59b6" white "QUIZ"
btn   "$DIR/btn_puzzle.png"    300x120  "#2980b9" white "Puzzle"
btn_h "$DIR/btn_puzzle_h.png"  300x120  "#3498db" white "Puzzle"
btn   "$DIR/btn_a.png"         150x80   "#ecf0f1" black "A"
btn_h "$DIR/btn_a_h.png"      150x80   "#bdc3c7" black "A"
btn   "$DIR/btn_b.png"         150x80   "#ecf0f1" black "B"
btn_h "$DIR/btn_b_h.png"      150x80   "#bdc3c7" black "B"
btn   "$DIR/btn_c.png"         150x80   "#ecf0f1" black "C"
btn_h "$DIR/btn_c_h.png"      150x80   "#bdc3c7" black "C"

echo "  [OK] engimemenu/assets"

# ────────────────────────────────────────────────────────────
# FONTS – copy Arial to every menu's assets/font.ttf
# ────────────────────────────────────────────────────────────
ARIAL=""
for candidate in \
    /usr/share/fonts/truetype/msttcorefonts/Arial.ttf \
    /usr/share/fonts/truetype/msttcorefonts/arial.ttf \
    /usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf \
    /usr/share/fonts/truetype/dejavu/DejaVuSans.ttf \
    /usr/share/fonts/TTF/Arial.ttf; do
    if [ -f "$candidate" ]; then
        ARIAL="$candidate"
        break
    fi
done

if [ -z "$ARIAL" ]; then
    echo "WARNING: Arial font not found. Install msttcorefonts:"
    echo "  sudo apt install ttf-mscorefonts-installer"
    echo "Skipping font copy."
else
    echo "=== Copying font: $ARIAL ==="
    for d in mainmenu optionmenu savemenu playermenu scoremenu engimemenu; do
        cp "$ARIAL" "$d/assets/font.ttf"
    done
    echo "  [OK] font.ttf copied to all menus"
fi

# ────────────────────────────────────────────────────────────
# SOUND EFFECTS – generate with ffmpeg (synthesised tones)
# ────────────────────────────────────────────────────────────
if ! command -v ffmpeg &>/dev/null; then
    echo "WARNING: ffmpeg not found – skipping sound generation."
    echo "  sudo apt install ffmpeg"
else
    echo "=== Generating sound effects with ffmpeg ==="

    # ── hover.wav  (short 0.1s click / blip at 1200 Hz) ──
    gen_hover() {
        ffmpeg -y -f lavfi -i "sine=frequency=1200:duration=0.1" \
            -af "afade=t=out:st=0.05:d=0.05" "$1" -loglevel error
    }

    # ── music.mp3  (main menu – 15s mellow ambient loop) ──
    gen_music() {
        ffmpeg -y -f lavfi \
            -i "sine=frequency=220:duration=15" \
            -f lavfi \
            -i "sine=frequency=330:duration=15" \
            -filter_complex \
            "[0]volume=0.3[a];[1]volume=0.2[b];[a][b]amix=inputs=2:duration=longest,afade=t=in:d=2,afade=t=out:st=13:d=2" \
            -b:a 128k "$1" -loglevel error
    }

    # ── music_option.mp3  (option/save/player – 15s different tone) ──
    gen_music_option() {
        ffmpeg -y -f lavfi \
            -i "sine=frequency=180:duration=15" \
            -f lavfi \
            -i "sine=frequency=270:duration=15" \
            -filter_complex \
            "[0]volume=0.25[a];[1]volume=0.15[b];[a][b]amix=inputs=2:duration=longest,afade=t=in:d=2,afade=t=out:st=13:d=2" \
            -b:a 128k "$1" -loglevel error
    }

    # ── victory.mp3  (score screen – 8s ascending celebratory tones) ──
    gen_victory() {
        ffmpeg -y -f lavfi \
            -i "sine=frequency=523:duration=2" \
            -f lavfi \
            -i "sine=frequency=659:duration=2" \
            -f lavfi \
            -i "sine=frequency=784:duration=2" \
            -f lavfi \
            -i "sine=frequency=1047:duration=2" \
            -filter_complex \
            "[0]adelay=0|0,afade=t=out:st=1.5:d=0.5[a]; \
             [1]adelay=2000|2000,afade=t=out:st=1.5:d=0.5[b]; \
             [2]adelay=4000|4000,afade=t=out:st=1.5:d=0.5[c]; \
             [3]adelay=6000|6000,afade=t=out:st=1.5:d=0.5[d]; \
             [a][b][c][d]amix=inputs=4:duration=longest" \
            -b:a 128k "$1" -loglevel error
    }

    # ── suspense.mp3  (enigme quiz – 10s low rumbling drum roll) ──
    gen_suspense() {
        ffmpeg -y -f lavfi \
            -i "sine=frequency=80:duration=10" \
            -f lavfi \
            -i "anoisesrc=d=10:c=pink:a=0.08" \
            -filter_complex \
            "[0]volume=0.4,tremolo=f=8:d=0.7[a];[1]volume=0.3[b];[a][b]amix=inputs=2:duration=longest,afade=t=in:d=1,afade=t=out:st=8:d=2" \
            -b:a 128k "$1" -loglevel error
    }

    # Generate hover.wav for all menus
    for d in mainmenu optionmenu savemenu playermenu scoremenu engimemenu; do
        gen_hover "$d/assets/hover.wav"
    done
    echo "  [OK] hover.wav (all menus)"

    # Main menu music
    gen_music "mainmenu/assets/music.mp3"
    echo "  [OK] mainmenu/assets/music.mp3"

    # Option/save/player share the same music
    gen_music_option "optionmenu/assets/music_option.mp3"
    cp "optionmenu/assets/music_option.mp3" "savemenu/assets/music_option.mp3"
    cp "optionmenu/assets/music_option.mp3" "playermenu/assets/music_option.mp3"
    echo "  [OK] music_option.mp3 (option/save/player)"

    # Victory music for score menu
    gen_victory "scoremenu/assets/victory.mp3"
    echo "  [OK] scoremenu/assets/victory.mp3"

    # Suspense music for enigme quiz
    gen_suspense "engimemenu/assets/suspense.mp3"
    echo "  [OK] engimemenu/assets/suspense.mp3"

    echo "  [OK] All sound effects generated"
fi

echo ""
echo "=== All assets generated! ==="
