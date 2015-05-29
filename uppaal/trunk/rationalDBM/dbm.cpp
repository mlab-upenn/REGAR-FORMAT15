
#include <vector>
#include <gmpxx.h>
#include <iostream>
#include "rationalDBM/dbm.hpp"
#include "rationalDBM/bound.hpp"

using std::vector;
using std::cout;
using std::ostream;
using std::endl;

namespace newdbm
{

    ostream& operator<< (ostream& o, const DBM& dbm)
    {
        for (int i= 0;i<dbm. numOfVars ();i++)
        {
            for (int j= 0;j<dbm.numOfVars();j++)
            {
                o << i << "-"<<j << dbm(i,j) << endl;
            }
        }
        return o;
    }
    DBM DBM::initZone (int i)
    {
        DBM dbm (i);
        dbm.initAll ();
        return dbm;
    }

    DBM::DBM(int size ) :  bounds (size, vector<Bound> (size ))
    {
        setDiagonal ();
        initLower ();
    }

    DBM::DBM (VariableAssignment v) : bounds (v.size(), vector<Bound> (v.size ()))
    {
        setDiagonal ();
        int size = v.size ();
        for (int i=0;i<size;i++)
        {
            setVar(i,v[i]);
        }
        // close ();
    }

    void DBM::setDiagonal ()
    {
        Bound zero (mpq_class (0),false);
        int size = numOfVars ();
        for (int i=0;i<size;i++)
        {
            setConstraint (i,i,zero);
        }
    }


    void DBM::initLower ()
    {
        Bound zero (mpq_class (0),false);
        int size = numOfVars ();
        for (int i=0;i<size;i++)
        {
            setConstraint (0,i,zero);
        }
    }


    void DBM::initUpper ()
    {
        Bound zero (mpq_class (0),false);
        for (int i=0;i<numOfVars ();i++)
        {
            setConstraint (i,0,zero);
        }
    }

    void DBM::initAll ()
    {
        Bound zero (mpq_class (0),false);
        for (int i=0;i<numOfVars ();i++)
        {
            for (int j=0;j< numOfVars ();j++)
            {
                setConstraint (i,j,zero);
            }
        }
    }

    Bound DBM::getBound (int i, int j) const
    {
        return bounds[i][j];
    }

    Bound DBM::operator() (int i,int j) const
    {

        return getBound(i,j);
    }

    //Floyd warshall classic shortest path algorithm
    void DBM::close ()
    {
        for (int i  = 0; i< bounds.size ();i++)
        {
            for (int j = 0; j < bounds.size ();j++)
            {
                for (int k = 0; k < bounds.size();k++)
                {
                    Bound ij = getBound(i,j);
                    Bound ikPluskj = (getBound (i,k))+getBound (k,j);
                    if (ikPluskj < ij)
                        setConstraint (i,j,ikPluskj);
                }
            }
        }
    }

    DBM DBM::intersect (const DBM& oth) const
    {

        DBM nDBM (numOfVars());
        int size = numOfVars ();
        for (int i=0;i<size;i++)
        {
            for (int j=0;j<size;j++)
            {
                if (getBound (i,j)< oth.getBound (i,j))
                {

                    nDBM.setConstraint (i,j,getBound (i,j));
                }
                else
                {

                    nDBM.setConstraint (i,j,oth.getBound (i,j));
                }
            }

        }
        nDBM.close ();
        return nDBM;
    }

    void DBM::reset (int x)
    {

        //cout << "Resetting " << x << endl;
        setVar (x,mpq_class(0));
    }


    DBM DBM::reset(const vector<int>& resets) const
    {
        DBM nDBM (*this);
        for (int i=0;i<resets.size();i++)
        {
            nDBM.reset (resets[i]);
        }

        return nDBM;
    }

    void DBM::free(int x)
    {
        Bound inf;
        //cout << "freeing" << endl;
        for (int i=0;i< numOfVars ();i++)
        {
            if (i!=x)
            {
                setConstraint (x,i,inf);
                setConstraint (i,x,getBound (i,0));
            }
        }
    }

    DBM DBM::free (const vector<int>& frees) const
    {
        DBM nDBM (*this);
        for (int i=0;i<frees.size();i++)
        {
            nDBM.free (frees [i]);
        }

        return nDBM;
    }



    DBM DBM::down () const
    {
        DBM nDBM (*this);
        Bound zeroBound (mpq_class(0),false);
        for (int i=1;i<numOfVars();i++)
        {
            nDBM.setConstraint (0,i,zeroBound);
            for (int j=1;j<numOfVars ();j++)
            {
                if (getBound(j,i)  < getBound (0,i))
                    nDBM.setConstraint (0,i,getBound(j,i));
            }
        }
        return nDBM;
    }

    DBM DBM::up () const
    {
        Bound inf;
        DBM nDBM(*this);
        for (int i=1;i<numOfVars ();i++)
        {
            nDBM.setConstraint(i,0,inf);
        }
        nDBM.close();
        return nDBM;
    }

    void DBM::constrain (int x, int y,Bound toConstrain)
    {

        if (toConstrain < getBound (x,y))
        {
            setConstraint (x,y,toConstrain);
            for (int i=0;i<numOfVars();i++)
            {
                for (int j=0;j<numOfVars ();j++)
                {

                    if ((getBound(i,x)+getBound (x,j)) < getBound (i,j))
                        setConstraint (i,j,(getBound(i,x)+getBound (x,j)));
                    if ((getBound (i,y)+getBound (y,j))<getBound (i,j))
                        setConstraint (i,j,(getBound(i,y)+getBound (y,j)));
                }
            }
        }

    }

    void DBM::setConstraint (int x,int y, Bound constraint)
    {
        bounds [x][y] = constraint;
    }

    void DBM::setVar (int x, mpq_class val)
    {
        Bound pos (val,false);
        Bound neg (-1*val,false);
        int size = numOfVars ();
        for (int i=0;i<size;i++)
        {
            setConstraint (x,i,pos+getBound (0,i));
            setConstraint (i,x,getBound (i,0)+neg);
        }
    }

    int DBM::numOfVars ()const
    {
        return bounds.size ();
    }

//If one of our constraints is looser than others , then we are not a subset
    bool DBM::subset (DBM oth) const
    {
        for (int i = 0;i<numOfVars ();i++)
        {
            for (int j=0;j<numOfVars ();j++)
            {
                if (getBound (i,j) > oth.getBound (i,j))
                    return false;
            }
        }

        return true;
    }
    //If one of the constraints in the diagonal is neg - then we are inconsistent
    bool DBM::consistent () const
    {
        Bound neg (mpq_class(0),true);
        for (int i=0;i<numOfVars ();i++)
        {
            if (getBound (i,i)<=neg)
                return true;
        }
        return false;
    }
    //Find point algorithms
    void DBM::findPointCore (int clockToSet,Bound& upper, Bound&lower, vector<int>& restricted, VariableAssignment& assigned)const
    {
        upper = getBound (clockToSet,0);
        lower = getBound (0,clockToSet);
        for (int i=0;i<restricted.size ();i++)
        {
            Bound upBuf = Bound (assigned[restricted[i]],false)+getBound (clockToSet,restricted[i]);
            Bound lowBuf = Bound (-1*assigned[restricted[i]],false)+getBound (restricted[i],clockToSet);

            upper = (upBuf < upper) ? (upBuf) : upper;
            lower = (lowBuf < lower) ? (lowBuf) : lower;
        }

    }
  
  VariableAssignment DBM::findPointMinClock (int clock, mpq_class epsilon)const {
    
    VariableAssignment assigns (numOfVars ());
    vector<int> restricted;
    Bound upper,lower;
    upper = getBound (clock,0);
    lower = getBound (0,clock);
    assigns[clock] = Bound::chooseValueMin (upper,lower,epsilon);
    restricted.push_back (clock);
    for (int i=0;i<numOfVars();i++)
      {
	if (i== clock)
	  continue;

	findPointCore (i,upper,lower,restricted,assigns);
	
	assigns[i] = Bound::chooseValueBetween (upper,lower,epsilon);
	restricted.push_back (i);
      }

    return assigns;
  }

    
  VariableAssignment DBM::findPointMin (mpq_class epsilon) const {
    VariableAssignment assigns (numOfVars ());
        vector<int> restricted;
        Bound upper,lower;
        for (int i=0;i<numOfVars();i++)
        {

	  findPointCore (i,upper,lower,restricted,assigns);
	    
	  assigns[i] = Bound::chooseValueMin (upper,lower,epsilon);
	  restricted.push_back (i);
        }
	
	
        return assigns;
  }

  
  VariableAssignment DBM::findPointMax (mpq_class epsilon) const {
    VariableAssignment assigns (numOfVars ());
        vector<int> restricted;
        Bound upper,lower;
        for (int i=0;i<numOfVars();i++)
        {

	  findPointCore (i,upper,lower,restricted,assigns);
	    
	  assigns[i] = Bound::chooseValueMax (upper,lower,epsilon);
	  restricted.push_back (i);
        }
	
	
        return assigns;
  }

    VariableAssignment DBM::findPoint (mpq_class epsilon) const
    {
        VariableAssignment assigns (numOfVars ());
        vector<int> restricted;
        Bound upper,lower;
        assigns[0]=mpq_class(0);
        restricted.push_back(0);

        for (int i=1;i<numOfVars();i++)
        {

            findPointCore (i,upper,lower,restricted,assigns);

            assigns[i] = Bound::chooseValueBetween (upper,lower,epsilon);
            restricted.push_back (i);
        }


        return assigns;
    }

  bool DBM::contained (const VariableAssignment& c) const {
    for (int i=0;i<numOfVars ();i++) {
      for (int j=0;j<numOfVars ();j++) {
	if (i!=0) {
	  mpq_class val = c[i] - c[j];
	  mpq_class val2 = c[j] - c[i];
	  if (!getBound (i,j).satisfied (val) || !getBound (j,i).satisfied (val2))
	    return false;
	  
	  
	}
      }
    }
    return true;
  }



}
