/*
 * Version 1.2 => Traduce la idea de forma imperativa a una orientada por Objeto
 */

 #include "Game.cpp"

 int main()
 {
     AGame *tetrisGame = new Game();
     return tetrisGame->Run();
 }