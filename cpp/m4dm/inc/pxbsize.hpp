#ifndef __PXBSIZE_HPP__
#define __PXBSIZE_HPP__

// tamaño del bloque que se guarda en el user space = sizeof (proxy_block)
// se pone aquí cableado para evitar dependencias con el archivo px_block.hpp
// hay un assert en px_block.cpp para asegurar que M4_SIZEOF_*BCONNECTOR = sizeof (proxy_block)

//a extinguir:
const int M4_SIZEOF_RBCONNECTOR = 0;
const int M4_SIZEOF_BBCONNECTOR = 0;

#endif //__PXBSIZE_HPP__
