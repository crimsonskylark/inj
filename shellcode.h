#pragma once


static constexpr char shellcode[ ]
{
    "\x55\x8B\xEC\x83\xEC\x50\xD9\x05\x0C\x20\x00\x10\x8D\x45\xB0\xD9\x15\xDC\xDD\xBB\xAA\xD9\x05\x10\x20\x00\x10\xD9\x15\xE0\xDD\xBB\xAA\xD9\x05\x14\x20\x00\x10\xD9\x15\xE4\xDD\xBB\xAA\xD9\x1D\xD8\xDD\xBB\xAA\xD9\xC9\x66\xC7\x05\xCC\xDD\xBB\xAA\x01\x01\xD9\x1D\xD0\xDD\xBB\xAA\x50\xD9\x1D\xD4\xDD\xBB\xAA\xD9\xEE\x8D\x45\xF4\xD9\x55\xF4\x6A\x00\xD9\x55\xF8\xD9\x55\xFC\x6A\x00\x50\xD9\x55\xE8\x8D\x45\xE8\xD9\x55\xEC\x50\xB8\xEF\xBE\xAD\xDE\xD9\x5D\xF0\xFF\xD0\x83\xC4\x14\xC9\xC3"
};