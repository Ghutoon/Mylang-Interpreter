#include <stdio.h>
#include <math.h>
#include "mylang.h"
#include "mylang.tab.h"
exReturn ex(nodeType *p)
{
    exReturn x;
    x.iORf = NULL;
    x.inti = 0;
    if (!p)
    {
        exReturn temp;
        temp.iORf = NULL;
        temp.inti = 0;
        return temp;
    }
    switch (p->type)
    {
    case typeCon:;
        exReturn temp;
        temp.iORf = p->con.iORf;
        if (p->con.iORf)
            temp.inti = p->con.ival;
        else
            temp.floatie = p->con.fval;

        return temp;

    case typeId:
        temp.iORf = p->id.iORf;
        if (temp.iORf)
            temp.inti = symInt[p->id.iIndex];
        else
            temp.floatie = symFloat[p->id.fIndex];
        return temp;

    case typeOpr:
        switch (p->opr.oper)
        {
        case WHILE:
            temp.iORf = NULL;
            temp.inti = 0;
            while (ex(p->opr.op[0]).inti)
                ex(p->opr.op[1]);
            return temp;

        case IF:
            temp.iORf = NULL;
            temp.inti = 0;
            if (ex(p->opr.op[0]).inti)
                ex(p->opr.op[1]);
            else if (p->opr.nops > 2)
                ex(p->opr.op[2]);
            return temp;

        case DISPLAY:
            temp = ex(p->opr.op[0]); // p->opr.op[0] stores pointer to a55;
            if (temp.iORf)
                printf("%d\n", temp.inti);
            else
                printf("%lf\n", temp.floatie);
            temp.iORf = NULL;
            temp.inti = 0;
            return temp;

        case ';':
            ex(p->opr.op[0]);
            return ex(p->opr.op[1]);
        case '=': // assigning shit to floating point numbers messes things up for some reason
            temp = ex(p->opr.op[1]);
            bool temptemp = p->opr.op[0]->id.iORf;
            if (p->opr.op[0]->id.iORf) // variable to which we are writing is integer
            {
                if (temp.iORf) // value we are writing is integer
                    symInt[p->opr.op[0]->id.iIndex] = (int)temp.inti;
                else
                    symInt[p->opr.op[0]->id.iIndex] = (double)temp.floatie;
            }
            else
            {
                if (temp.iORf)
                    symFloat[p->opr.op[0]->id.fIndex] = (double)temp.inti;
                else
                    symFloat[p->opr.op[0]->id.fIndex] = (double)temp.floatie;
            }
            return temp;

        case UMINUS:
            temp;
            temp = ex(p->opr.op[0]);
            if (temp.iORf)
                temp.inti = temp.inti * -1;
            else
                temp.floatie = temp.floatie * -1;
            return temp;

        case '+':;
            exReturn a, b;
            a = ex(p->opr.op[0]);
            b = ex(p->opr.op[1]);
            temp.iORf = a.iORf && b.iORf;

            if (a.iORf) // a contains integer variable
            {
                if (b.iORf) // b contains integer variable
                    temp.inti = a.inti + b.inti;
                else
                    temp.floatie = a.inti + b.floatie;
            }
            else
            {
                if (b.iORf) // b contains integer variable
                    temp.floatie = a.floatie + b.inti;
                else
                    temp.floatie = a.floatie + b.floatie;
            }
            return temp;

        case '-':
            a = ex(p->opr.op[0]);
            b = ex(p->opr.op[1]);
            temp.iORf = a.iORf && b.iORf;

            if (a.iORf)
            {
                if (b.iORf)
                    temp.inti = a.inti - b.inti;
                else
                    temp.floatie = a.inti - b.floatie;
            }
            else
            {
                if (b.iORf)

                    temp.floatie = a.floatie - b.inti;
                else
                    temp.floatie = a.floatie - b.floatie;
            }
            return temp;

        case '*':
            a = ex(p->opr.op[0]);
            b = ex(p->opr.op[1]);
            temp.iORf = a.iORf && b.iORf;

            if (a.iORf)
            {
                if (b.iORf) // b contains integer variable
                    temp.inti = a.inti * b.inti;
                else
                    temp.floatie = (double)a.inti * b.floatie;
            }
            else
            {
                if (b.iORf) // b contains integer variable
                    temp.floatie = a.floatie * (double)b.inti;
                else
                    temp.floatie = a.floatie * b.floatie;
            }
            return temp;

        case '/':
            a = ex(p->opr.op[0]);
            b = ex(p->opr.op[1]);
            temp.iORf = a.iORf && b.iORf;

            if (a.iORf)

            {
                if (b.iORf)
                    temp.inti = a.inti / b.inti;
                else
                    temp.floatie = a.inti / b.floatie;
            }
            else
            {
                if (b.iORf)
                    temp.floatie = a.floatie / b.inti;
                else
                    temp.floatie = a.floatie / b.floatie;
            }
            return temp;

        case '^':
            a = ex(p->opr.op[0]);
            b = ex(p->opr.op[1]);
            temp.iORf = a.iORf && b.iORf;

            if (a.iORf)
            {
                if (b.iORf)
                    temp.inti = pow(a.inti, b.inti);
                else
                    temp.floatie = pow(a.inti, b.floatie);
            }
            else
            {
                if (b.iORf)
                    temp.floatie = pow(a.floatie, b.inti);
                else
                    temp.floatie = pow(a.floatie, b.floatie);
            }
            return temp;

        case '%':
            a = ex(p->opr.op[0]);
            b = ex(p->opr.op[1]);
            temp.iORf = true;

            if (a.iORf)
            {
                if (b.iORf)
                    temp.inti = a.inti % b.inti;
                else
                    temp.inti = a.inti % (int)b.floatie;
            }
            else
            {
                if (b.iORf)
                    temp.inti = (int)a.floatie % (int)b.inti;
                else
                    temp.inti = (int)a.floatie % (int)b.floatie;
            }
            return temp;

        case '<':
            a = ex(p->opr.op[0]);
            b = ex(p->opr.op[1]);
            temp.iORf = NULL;

            if (a.iORf)
            {
                if (b.iORf)
                    temp.inti = a.inti < b.inti;
                else
                    temp.inti = a.inti < b.floatie;
            }
            else
            {
                if (b.iORf)
                    temp.inti = a.floatie < b.inti;
                else
                    temp.inti = a.floatie < b.floatie;
            }
            return temp;
        case '>':
            a = ex(p->opr.op[0]);
            b = ex(p->opr.op[1]);
            temp.iORf = NULL;

            if (a.iORf)
            {
                if (b.iORf)
                    temp.inti = a.inti > b.inti;
                else
                    temp.inti = a.inti > b.floatie;
            }
            else
            {
                if (b.iORf)
                    temp.inti = a.floatie > b.inti;
                else
                    temp.inti = a.floatie > b.floatie;
            }
            return temp;
        case GE:
            a = ex(p->opr.op[0]);
            b = ex(p->opr.op[1]);
            temp.iORf = NULL;

            if (a.iORf)
            {
                if (b.iORf)
                    temp.inti = a.inti >= b.inti;
                else
                    temp.inti = a.inti >= b.floatie;
            }
            else
            {
                if (b.iORf)
                    temp.inti = a.floatie >= b.inti;
                else
                    temp.inti = a.floatie >= b.floatie;
            }
            return temp;
        case LE:
            a = ex(p->opr.op[0]);
            b = ex(p->opr.op[1]);
            temp.iORf = NULL;

            if (a.iORf)
            {
                if (b.iORf)
                    temp.inti = a.inti <= b.inti;
                else
                    temp.inti = a.inti <= b.floatie;
            }
            else
            {
                if (b.iORf)
                    temp.inti = a.floatie <= b.inti;
                else
                    temp.inti = a.floatie <= b.floatie;
            }
            return temp;

        case NE:
            a = ex(p->opr.op[0]);
            b = ex(p->opr.op[1]);
            temp.iORf = NULL;

            if (a.iORf)
            {
                if (b.iORf)
                    temp.inti = a.inti != b.inti;
                else
                    temp.inti = a.inti != b.floatie;
            }
            else
            {
                if (b.iORf)
                    temp.inti = a.floatie != b.inti;
                else
                    temp.inti = a.floatie != b.floatie;
            }
            return temp;
        case EQ:
            a = ex(p->opr.op[0]);
            b = ex(p->opr.op[1]);
            temp.iORf = NULL;

            if (a.iORf)
            {
                if (b.iORf)
                    temp.inti = a.inti == b.inti;
                else
                    temp.inti = a.inti == b.floatie;
            }
            else
            {
                if (b.iORf)
                    temp.inti = a.floatie == b.inti;
                else
                    temp.inti = a.floatie == b.floatie;
            }
            return temp;
        }
    }
    return x;
}
