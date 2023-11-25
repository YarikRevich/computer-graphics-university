#include <math.h>
#include <SDL2/SDL.h>

/**
 * Сontains tool-set used by both encoder and decoder.
*/
class Tools {
public:
    /**
     * Converts given 24 bit RGB color to 7 bit RGB single.
     * @param color - given 24 bit RGB color to be converted.
     * @return 
    */
    Uint8 convert24BitRGBTo7BitRGB(SDL_Color color);

    /**
     * Converts given 7 bit single RGB to 24 bit RGB color.
     * @param color - given 7 bit single color to be converted.
     * @return
    */
    SDL_Color convert7BitRGBTo24BitRGB(Uint8 color);

    /**
     * Converts given 24 bit RGB color to 7 bit grey single. 
     * @param color
     * @return converted 7 bit grey single color.
    */
    Uint8 convert24BitRGBTo7BitGrey(SDL_Color color);

    /**
     * Converts 7 bit grey single to 24 bit RGB color.
    */
    SDL_Color convert7BitGreyTo24BitRGB(Uint8 grey);

    /**
     * Normilizes given value according to the given min and max limiters.
     * @param value - value to be normilized.
     * @param min - min value in the range.
     * @param max - max value in the range.
     * @return normilized value.
    */
    int normalizeValue(int value, int min, int max);
};

// void Funkcja1() {

//     SDL_Color kolor;
//     int R, G, B;
//     int nowyR, nowyG, nowyB;

//     for (int y=0; y<wysokosc/2; y++){
//         for(int x=0; x<szerokosc/2; x++){
//             kolor=getPixel(x, y);
//             R=kolor.r;
//             G=kolor.g;
//             B=kolor.b;

//             //Wersja 1
//             nowyR=R>>6;
//             nowyG=G>>5;
//             nowyB=B>>6;

//             //cout<<nowyR<<","<<nowyG<<","<<nowyB<<" ";

//             R = nowyR<<6;
//             G = nowyG<<5;
//             B = nowyB<<6;

//             setPixel(x+szerokosc/2, y, R, G, B);
//         }
//     }

//     SDL_UpdateWindowSurface(window);
// }

// void Funkcja2() {

//     SDL_Color kolor;
//     int R, G, B;
//     int nowyR, nowyG, nowyB;

//     for (int y=0; y<wysokosc/2; y++){
//         for(int x=0; x<szerokosc/2; x++){
//             kolor=getPixel(x, y);
//             R=kolor.r;
//             G=kolor.g;
//             B=kolor.b;

//             //Wersja 2
//             nowyR=R>>6;
//             nowyG=G>>5;
//             nowyB=B>>6;

//             //cout<<nowyR<<","<<nowyG<<","<<nowyB<<" ";

//             R=nowyR * 255.0/3.0;
//             G=nowyG * 255.0/7.0;
//             B=nowyB * 255.0/3.0;

//             setPixel(x, y+wysokosc/2, R, G, B);
//         }
//     }

//     SDL_UpdateWindowSurface(window);
// }

// void Funkcja3() {

//     SDL_Color kolor;
//     int R, G, B;
//     int nowyR, nowyG, nowyB;

//     for (int y=0; y<wysokosc/2; y++){
//         for(int x=0; x<szerokosc/2; x++){
//             kolor=getPixel(x, y);
//             R=kolor.r;
//             G=kolor.g;
//             B=kolor.b;

//             //Wersja 4
//             nowyR=round(R*3.0/255.0);
//             nowyG=round(G*7.0/255.0);
//             nowyB=round(B*3.0/255.0);

//             //cout<<nowyR<<","<<nowyG<<","<<nowyB<<" ";

//             R=nowyR * 255.0/3.0;
//             G=nowyG * 255.0/7.0;
//             B=nowyB * 255.0/3.0;

//             setPixel(x+szerokosc/2, y+wysokosc/2, R, G, B);
//         }
//     }

//     SDL_UpdateWindowSurface(window);
// }

// void Funkcja4() {

//     SDL_Color kolor, nowyKolor;
//    Uint8 kolor7bit;

//     for (int y=0; y<wysokosc/2; y++){
//         for(int x=0; x<szerokosc/2; x++){
//             kolor=getPixel(x, y);

//             kolor7bit=z24RGBna7RGB(kolor);
//             nowyKolor=z7RGBna24RGB(kolor7bit);

//             setPixel(x+szerokosc/2, y+wysokosc/2, nowyKolor.r, nowyKolor.g, nowyKolor.b);
//         }
//     }
//     SDL_UpdateWindowSurface(window);
// }

// void Funkcja5() {

//     SDL_Color kolor, nowyKolor;
//     Uint8 szary7bit;

//     for (int y=0; y<wysokosc/2; y++){
//         for(int x=0; x<szerokosc/2; x++){
//             kolor=getPixel(x, y);

//             szary7bit=z24RGBna7BW(kolor);
//             nowyKolor=z7BWna24RGB(szary7bit);

//             setPixel(x+szerokosc/2, y+wysokosc/2, nowyKolor.r, nowyKolor.g, nowyKolor.b);
//         }
//     }

//     SDL_UpdateWindowSurface(window);
// }

// void Funkcja6() {

//     SDL_Color kolor, tymczasowyKolor, nowyKolor;
//     Uint8 szary7bit;

//     Uint8 szary, nowySzary;
//     int tymczasowySzary;


//     int przesuniecie = 1;
//     float bledy[(szerokosc/2)+2][(wysokosc/2)+2];
//     memset(bledy,0,sizeof(bledy));
//     int blad=0;

//     for (int y=0; y<wysokosc/2; y++){
//         for(int x=0; x<szerokosc/2; x++){
//             kolor=getPixel(x, y);
//             szary=0.299*kolor.r+0.587*kolor.g+0.114*kolor.b;
//             tymczasowySzary=szary+bledy[x+przesuniecie][y];
//             tymczasowySzary=normalizacja(tymczasowySzary,0,255);

//             tymczasowyKolor.r=tymczasowySzary;
//             tymczasowyKolor.g=tymczasowySzary;
//             tymczasowyKolor.b=tymczasowySzary;

//             szary7bit=z24RGBna7BW(tymczasowyKolor);
//             nowyKolor=z7BWna24RGB(szary7bit);

//             nowySzary=nowyKolor.r;

//             blad=tymczasowySzary-nowySzary;

//             setPixel(x+szerokosc/2, y+wysokosc/2, nowyKolor.r, nowyKolor.g, nowyKolor.b);

//             bledy[x+1+przesuniecie][y]+=(blad*7.0/16.0);
//             bledy[x-1+przesuniecie][y+1]+=(blad*3.0/16.0);
//             bledy[x+przesuniecie][y+1]+=(blad*5.0/16.0);
//             bledy[x+1+przesuniecie][y+1]+=(blad*1.0/16.0);
//         }
//     }

//     SDL_UpdateWindowSurface(window);
// }

// void Funkcja7() {

//     SDL_Color kolor, tymczasowyKolor, nowyKolor;
//     Uint8 kolor7bit;

//     Uint8 nowyCzerwony, nowyZielony, nowyNiebieski;
//     int tymczasowyCzerwony, tymczasowyZielony, tymczasowyNiebieski;


//     int przesuniecie = 1;
//     float bledyCzerwony[(szerokosc/2)+2][(wysokosc/2)+2];
//     float bledyZielony[(szerokosc/2)+2][(wysokosc/2)+2];
//     float bledyNiebieski[(szerokosc/2)+2][(wysokosc/2)+2];
//     memset(bledyCzerwony,0,sizeof(bledyCzerwony));
//     memset(bledyZielony,0,sizeof(bledyZielony));
//     memset(bledyNiebieski,0,sizeof(bledyNiebieski));
//     int bladCzerwony, bladZielony=0, bladNiebieski=0;

//     for (int y=0; y<wysokosc/2; y++){
//         for(int x=0; x<szerokosc/2; x++){
//             kolor=getPixel(x, y);

//             tymczasowyCzerwony=kolor.r+bledyCzerwony[x+przesuniecie][y];
//             tymczasowyCzerwony=normalizacja(tymczasowyCzerwony,0,255);
//             tymczasowyKolor.r=tymczasowyCzerwony;

//             tymczasowyZielony=kolor.g+bledyZielony[x+przesuniecie][y];
//             tymczasowyZielony=normalizacja(tymczasowyZielony,0,255);
//             tymczasowyKolor.g=tymczasowyZielony;

//             tymczasowyNiebieski=kolor.b+bledyNiebieski[x+przesuniecie][y];
//             tymczasowyNiebieski=normalizacja(tymczasowyNiebieski,0,255);
//             tymczasowyKolor.b=tymczasowyNiebieski;

//             kolor7bit=z24RGBna7RGB(tymczasowyKolor);
//             nowyKolor=z7RGBna24RGB(kolor7bit);

//             nowyCzerwony=nowyKolor.r;
//             nowyZielony=nowyKolor.g;
//             nowyNiebieski=nowyKolor.b;

//             bladCzerwony=tymczasowyCzerwony-nowyCzerwony;
//             bladZielony=tymczasowyZielony-nowyZielony;
//             bladNiebieski=tymczasowyNiebieski-nowyNiebieski;

//             setPixel(x+szerokosc/2, y+wysokosc/2, nowyKolor.r, nowyKolor.g, nowyKolor.b);

//             bledyCzerwony[x+1+przesuniecie][y]+=(bladCzerwony*7.0/16.0);
//             bledyCzerwony[x-1+przesuniecie][y+1]+=(bladCzerwony*3.0/16.0);
//             bledyCzerwony[x+przesuniecie][y+1]+=(bladCzerwony*5.0/16.0);
//             bledyCzerwony[x+1+przesuniecie][y+1]+=(bladCzerwony*1.0/16.0);

//             bledyZielony[x+1+przesuniecie][y]+=(bladZielony*7.0/16.0);
//             bledyZielony[x-1+przesuniecie][y+1]+=(bladZielony*3.0/16.0);
//             bledyZielony[x+przesuniecie][y+1]+=(bladZielony*5.0/16.0);
//             bledyZielony[x+1+przesuniecie][y+1]+=(bladZielony*1.0/16.0);

//             bledyNiebieski[x+1+przesuniecie][y]+=(bladNiebieski*7.0/16.0);
//             bledyNiebieski[x-1+przesuniecie][y+1]+=(bladNiebieski*3.0/16.0);
//             bledyNiebieski[x+przesuniecie][y+1]+=(bladNiebieski*5.0/16.0);
//             bledyNiebieski[x+1+przesuniecie][y+1]+=(bladNiebieski*1.0/16.0);
//         }
//     }

//     SDL_UpdateWindowSurface(window);
// }