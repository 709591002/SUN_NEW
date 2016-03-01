#pragma once
/*
definition of different types of errors
*/

#ifndef	__ERROR_H__
#define	__ERROR_H__

#define   LINKITSELF             1   /*a node linking with itself*/
#define   REDUNDANTLINK    2   /* more than one links exist in the same node pairs*/
#define   BEYONDLIMIT         3   /*beyond limit*/
#define   UNCONNECTED       4   /*network is unconnected*/
#define   WRONGCLUSTER    5   /*wrong cluster*/
#define   UNKNOWNMODEL  6   /*unknown network model*/
#define   NOLINK                   7   /*link doesn't exist in the network*/
#define   WRONGPARAMETER   8 /*uncorrected network modeling paramter*/

#define   NOCLUSTER             10  
#define   NOCLUSTEDNODE             11 


#endif	/* __ERROR_H__ */
