#pragma once
#ifndef __DEFS_H
#define __DEFS_H

#ifdef BUILD_CORE_DLL 
#define CORE_API __declspec(dllexport)  
#else  
#define CORE_API __declspec(dllimport)  
#endif  

#endif