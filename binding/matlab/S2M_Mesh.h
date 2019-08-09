#ifndef MATLAB_S2M_MESH_H
#define MATLAB_S2M_MESH_H

#include <mex.h>
#include "s2mMusculoSkeletalModel.h"
#include "class_handle.h"
#include "processArguments.h"

void S2M_Mesh( int, mxArray *plhs[],
                  int nrhs, const mxArray*prhs[] ){

    // Verifier les arguments d'entree
    checkNombreInputParametres(nrhs, 3, 4, "3 arguments are required (+1 optional) where the 2nd is the handler on the model, 3rd is the Q and 4th optional is a specific segment index");
    // Recevoir le model
    s2mMusculoSkeletalModel * model = convertMat2Ptr<s2mMusculoSkeletalModel>(prhs[1]);
    unsigned int nQ = model->nbQ(); /* Get the number of DoF */

    // Recevoir Q
    s2mGenCoord Q = *getParameterQ(prhs, 2, nQ).begin();

    // Recevoir l'index (si envoye)
    int idx(-1);
    if (nrhs==4)
        idx = getInteger(prhs,3)-1; // -1 afin que le segment 1 soit le root

    // Output
    if ( idx==-1){ // Si on a demande tous les segments
        // Trouver ou sont les marqueurs
        std::vector<std::vector<s2mNodeBone>> allMesh(model->meshPoints(Q));

        // Create a matrix for the return argument
        plhs[0] = mxCreateCellMatrix( allMesh.size(), 1);
        for (unsigned int i_bone=0; i_bone<allMesh.size(); ++i_bone){
            mxArray *mesh_out_tp = mxCreateDoubleMatrix( 3, (*(allMesh.begin()+i_bone)).size(), mxREAL);
            double *Mesh = mxGetPr(mesh_out_tp);

            // Remplir le output
            std::vector<s2mNodeBone>::iterator it=(*(allMesh.begin()+i_bone)).begin();
            for (unsigned int i=0; (it+i)!=(*(allMesh.begin()+i_bone)).end(); ++i){
                Mesh[i*3] = (*(it+i))(0);
                Mesh[i*3+1] = (*(it+i))(1);
                Mesh[i*3+2] = (*(it+i))(2);
            }
            mxSetCell(plhs[0],i_bone,mesh_out_tp);
        }
        return;

    }
    else{ // Si on a demande un segment precis
        std::vector<s2mNodeBone> Mesh_tp(model->meshPoints(Q,static_cast<unsigned int>(idx)));

        // Create a matrix for the return argument
        plhs[0] = mxCreateDoubleMatrix(3, Mesh_tp.size(), mxREAL);
        double *Mesh = mxGetPr(plhs[0]);

        // Remplir le output
        std::vector<s2mNodeBone>::iterator it=Mesh_tp.begin();
        for (unsigned int i=0; (it+i)!=Mesh_tp.end(); ++i){
            Mesh[i*3] = (*(it+i))(0);
            Mesh[i*3+1] = (*(it+i))(1);
            Mesh[i*3+2] = (*(it+i))(2);
        }
        return;
    }
}

#endif // MATLAB_S2M_MESH_H