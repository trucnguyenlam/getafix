// ####    ###   #   #  #   #  ####            #
// #   #  #   #  #   #  #   #   #  #          # #
// #   #  #   #  #   #  ##  #   #  #         #   #
// ####   #   #  #   #  # # #   #  #         #   #
// # #    #   #  #   #  #  ##   #  #         #   #
// #  #   #   #  #   #  #   #   #  #          # #
// #   #   ###    ###   #   #  ####            #

// Forward abstraction

// ###   #  #  ###   ####   ##   ###          #
//  #    #  #  #  #  #     #  #  #  #        ##
//  #    ####  #  #  ###   #  #  #  #         #
//  #    #  #  ###   #     ####  #  #         #
//  #    #  #  # #   #     #  #  #  #         #
//  #    #  #  #  #  ####  #  #  ###         ###

mu bool Thread1_0_Forward_AbsLocalKeepG(
 PrCount    s_pc1,                  // Program counter
 Local      s_CL1,                  // Local variable
 PrCount    s_pc2,                  // Program counter
 Local      s_CL2,                  // Local variable
 PrCount    s_pc3,                  // Program counter
 Local      s_CL3,                  // Local variable
 Global     s_G                     // Global variable
)
 s_pc1    <  s_CL1,
 s_CL1    <  s_pc2,
 s_pc2    <  s_CL2,
 s_CL2    <  s_pc3,
 s_pc3    <  s_CL3,
 s_CL3    <  s_G
( false

  // initial configuration (init)
  |  (
           initPC(s_pc1)              // Initial PC
         & initPC(s_pc2)              // Initial PC
         & initPC(s_pc3)              // Initial PC
         & GlobalInit(s_G)
         & Thread1_0_Backward_AbsLocal(s_pc1, s_CL1, s_pc2, s_CL2, s_pc3, s_CL3, s_G)
     )

//*********************************************************************************/
// forward propagation on internal transitions
//*********************************************************************************/

  |  (
         true
      & (exists                  // There exists an internal state that
           PrCount t_pc1,
           Local   t_CL1,
           Global t_G.
           (    (   Thread1_0_Forward_AbsLocalKeepG( t_pc1, t_CL1, s_pc2, s_CL2,s_pc3, s_CL3, t_G )
                )
               &(
                  (
                    programInt1_1(t_pc1,s_pc1,t_CL1,s_CL1,t_G,s_G)      // Assignment related
                    & CopyVariables_ProgramInt_1(t_pc1,t_CL1,s_CL1,t_G,s_G)   //  Copy others global variable
                  )
                  | programInt3(1, t_pc1, s_pc1, t_CL1, s_CL1, t_G, s_G )   // constrain
                )
           )
      )
    )

  | (
          true
       & (exists PrCount t_pc1.
             (     Thread1_0_Forward_AbsLocalKeepG( t_pc1, s_CL1, s_pc2, s_CL2,s_pc3, s_CL3, s_G )
                 & programInt2_1(t_pc1,s_pc1,s_CL1,s_G)    // Control flow statement
             )
         )
    )

);
#size Thread1_0_Forward_AbsLocalKeepG;
#timer;


 // #####  #   #  ####   #####    #    ####          ###
 //   #    #   #  #   #  #       # #    #  #        #   #
 //   #    #   #  #   #  #      #   #   #  #            #
 //   #    #####  ####   ####   #   #   #  #          ##
 //   #    #   #  # #    #      #####   #  #         #
 //   #    #   #  #  #   #      #   #   #  #        #
 //   #    #   #  #   #  #####  #   #  ####         #####
mu bool Thread2_0_Forward_AbsLocalKeepG(
 PrCount    s_pc1,                  // Program counter
 Local      s_CL1,                  // Local variable
 PrCount    s_pc2,                  // Program counter
 Local      s_CL2,                  // Local variable
 PrCount    s_pc3,                  // Program counter
 Local      s_CL3,                  // Local variable
 Global     s_G                    // Global variable
)
 s_pc1    <  s_CL1,
 s_CL1    <  s_pc2,
 s_pc2    <  s_CL2,
 s_CL2    <  s_pc3,
 s_pc3    <  s_CL3,
 s_CL3    <  s_G
( false

  // initial configuration (init)
  |  (  (exists
          Local   t_CL1,
          Local   t_CL2,
          Local   t_CL3.
          (
                 Thread1_0_Forward_AbsLocalKeepG( s_pc1, t_CL1, s_pc2, t_CL2, s_pc3, t_CL3, s_G )
              &  Thread2_0_Backward_AbsLocal( s_pc1, t_CL1, s_pc2, t_CL2, s_pc3, t_CL3, s_G )
          )
        )
              &  s_G.v1 = 0
        &  ContextSwitch_1(s_pc1)
     )


//*********************************************************************************/
// forward propagation on internal transitions
//*********************************************************************************/

  |  (
      true
      & (exists                  // There exists an internal state that
           PrCount t_pc2,
           Local   t_CL2,
           Global  t_G.
           (    (   Thread2_0_Forward_AbsLocalKeepG( s_pc1, s_CL1, t_pc2, t_CL2,s_pc3, s_CL3, t_G )
                )
               &(
                  ( programInt1_2(t_pc2,s_pc2,t_CL2,s_CL2,t_G,s_G)      // Assignment related
                    & CopyVariables_ProgramInt_2(t_pc2,t_CL2,s_CL2,t_G,s_G)   //  Copy others global variable
                  )
                  | programInt3(2, t_pc2, s_pc2, t_CL2, s_CL2, t_G, s_G )   // constrain
                )
           )
      )
    )

  | (
         true
       & (exists PrCount t_pc2.
             (     Thread2_0_Forward_AbsLocalKeepG( s_pc1, s_CL1, t_pc2, s_CL2,s_pc3, s_CL3, s_G )
                 & programInt2_2(t_pc2,s_pc2,s_CL2,s_G)    // Control flow statement
             )
         )
    )

);
#size Thread2_0_Forward_AbsLocalKeepG;
#timer;



// ###   #  #  ###   ####   ##   ###         ####
//  #    #  #  #  #  #     #  #  #  #           #
//  #    ####  #  #  ###   #  #  #  #         ##
//  #    #  #  ###   #     ####  #  #           #
//  #    #  #  # #   #     #  #  #  #        #  #
//  #    #  #  #  #  ####  #  #  ###          ##

mu bool Thread3_0_Forward_AbsLocalKeepG(
 PrCount    s_pc1,                  // Program counter
 Local      s_CL1,                  // Local variable
 PrCount    s_pc2,                  // Program counter
 Local      s_CL2,                  // Local variable
 PrCount    s_pc3,                  // Program counter
 Local      s_CL3,                  // Local variable
 Global     s_G                    // Global variable
)
 s_pc1    <  s_CL1,
 s_CL1    <  s_pc2,
 s_pc2    <  s_CL2,
 s_CL2    <  s_pc3,
 s_pc3    <  s_CL3,
 s_CL3    <  s_G
( false

  // initial configuration (init)
  |  (
        (exists
          Local   t_CL1,
          Local   t_CL2,
          Local   t_CL3.
          (
                 Thread2_0_Forward_AbsLocalKeepG( s_pc1, t_CL1, s_pc2, t_CL2, s_pc3, t_CL3, s_G )
              &  Thread3_0_Backward_AbsLocal( s_pc1, t_CL1, s_pc2, t_CL2, s_pc3, t_CL3, s_G )
          )
        )
              &  s_G.v1 = 0
        &  ContextSwitch_2(s_pc2)
     )


//*********************************************************************************/
// forward propagation on internal transitions
//*********************************************************************************/

  |  (
      true
      & (exists                  // There exists an internal state that
           PrCount t_pc3,
           Local   t_CL3,
           Global  t_G.
           (    (   Thread3_0_Forward_AbsLocalKeepG( s_pc1, s_CL1, s_pc2, s_CL2, t_pc3, t_CL3, t_G )
                )
               &(
                  ( programInt1_3(t_pc3,s_pc3,t_CL3,s_CL3,t_G,s_G)      // Assignment related
                    & CopyVariables_ProgramInt_3(t_pc3,t_CL3,s_CL3,t_G,s_G)   //  Copy others global variable
                  )
                  | programInt3(3, t_pc3, s_pc3, t_CL3, s_CL3, t_G, s_G )   // constrain
                )
           )
      )
    )

  | (
         true
       & (exists PrCount t_pc3.
             (     Thread3_0_Forward_AbsLocalKeepG( s_pc1, s_CL1, s_pc2, s_CL2,t_pc3, s_CL3, s_G )
                 & programInt2_3(t_pc3,s_pc3,s_CL3,s_G)    // Control flow statement
             )
         )
    )

);
#size Thread3_0_Forward_AbsLocalKeepG;
#timer;


// ####    ###   #   #  #   #  ####                   #
// #   #  #   #  #   #  #   #   #  #                 ##
// #   #  #   #  #   #  ##  #   #  #                # #
// ####   #   #  #   #  # # #   #  #                  #
// # #    #   #  #   #  #  ##   #  #                  #
// #  #   #   #  #   #  #   #   #  #                  #
// #   #   ###    ###   #   #  ####                 #####

// FORWARD ABSTRACTION

// ###   #  #  ###   ####   ##   ###          #
//  #    #  #  #  #  #     #  #  #  #        ##
//  #    ####  #  #  ###   #  #  #  #         #
//  #    #  #  ###   #     ####  #  #         #
//  #    #  #  # #   #     #  #  #  #         #
//  #    #  #  #  #  ####  #  #  ###         ###

mu bool Thread1_1_Forward_AbsLocalKeepG(
 PrCount    s_pc1,                  // Program counter
 Local      s_CL1,                  // Local variable
 PrCount    s_pc2,                  // Program counter
 Local      s_CL2,                  // Local variable
 PrCount    s_pc3,                  // Program counter
 Local      s_CL3,                  // Local variable
 Global     s_G                    // Global variable
)
 s_pc1    <  s_CL1,
 s_CL1    <  s_pc2,
 s_pc2    <  s_CL2,
 s_CL2    <  s_pc3,
 s_pc3    <  s_CL3,
 s_CL3    <  s_G
( false

  // initial configuration (init)
  |  (
        (exists
          Local   t_CL1,
          Local   t_CL2,
          Local   t_CL3.
          (
                 Thread3_0_Forward_AbsLocalKeepG( s_pc1, t_CL1, s_pc2, t_CL2, s_pc3, t_CL3, s_G )
              &  Thread1_1_Backward_AbsLocal( s_pc1, t_CL1, s_pc2, t_CL2, s_pc3, t_CL3, s_G )
          )
        )
              &  s_G.v1 = 0
        &  ContextSwitch_3(s_pc3)
     )

//*********************************************************************************/
// forward propagation on internal transitions
//*********************************************************************************/

  |  (
         true
      & (exists                  // There exists an internal state that
           PrCount t_pc1,
           Local   t_CL1,
           Global t_G.
           (    (   Thread1_1_Forward_AbsLocalKeepG( t_pc1, t_CL1, s_pc2, s_CL2,s_pc3, s_CL3, t_G )
                )
               &(
                  ( programInt1_1(t_pc1,s_pc1,t_CL1,s_CL1,t_G,s_G)      // Assignment related
                    & CopyVariables_ProgramInt_1(t_pc1,t_CL1,s_CL1,t_G,s_G)   //  Copy others global variable
                  )
                  | programInt3(1, t_pc1, s_pc1, t_CL1, s_CL1, t_G, s_G )   // constrain
                )
           )
      )
    )

  | (
         true
       & (exists PrCount t_pc1.
             (     Thread1_1_Forward_AbsLocalKeepG( t_pc1, s_CL1, s_pc2, s_CL2,s_pc3, s_CL3, s_G )
                 & programInt2_1(t_pc1,s_pc1,s_CL1,s_G)    // Control flow statement
             )
         )
    )

);
#size Thread1_1_Forward_AbsLocalKeepG;
#timer;

 // #####  #   #  ####   #####    #    ####          ###
 //   #    #   #  #   #  #       # #    #  #        #   #
 //   #    #   #  #   #  #      #   #   #  #            #
 //   #    #####  ####   ####   #   #   #  #          ##
 //   #    #   #  # #    #      #####   #  #         #
 //   #    #   #  #  #   #      #   #   #  #        #
 //   #    #   #  #   #  #####  #   #  ####         #####

mu bool Thread2_1_Forward_AbsLocalKeepG(
 PrCount    s_pc1,                  // Program counter
 Local      s_CL1,                  // Local variable
 PrCount    s_pc2,                  // Program counter
 Local      s_CL2,                  // Local variable
 PrCount    s_pc3,                  // Program counter
 Local      s_CL3,                  // Local variable
 Global     s_G                    // Global variable
)
 s_pc1    <  s_CL1,
 s_CL1    <  s_pc2,
 s_pc2    <  s_CL2,
 s_CL2    <  s_pc3,
 s_pc3    <  s_CL3,
 s_CL3    <  s_G
( false

  // initial configuration (init)
  |  (
        (exists
          Local   t_CL1,
          Local   t_CL2,
          Local   t_CL3.
          (
                 Thread1_1_Forward_AbsLocalKeepG( s_pc1, t_CL1, s_pc2, t_CL2, s_pc3, t_CL3, s_G )
              &  Thread2_1_Backward_AbsLocal( s_pc1, t_CL1, s_pc2, t_CL2, s_pc3, t_CL3, s_G )
          )
        )
              &  s_G.v1 = 0
        &  ContextSwitch_1(s_pc1)
     )

//*********************************************************************************/
// forward propagation on internal transitions
//*********************************************************************************/

  |  (
      true
      & (exists                  // There exists an internal state that
           PrCount t_pc2,
           Local   t_CL2,
           Global  t_G.
           (    (
                    Thread2_1_Forward_AbsLocalKeepG( s_pc1, s_CL1, t_pc2, t_CL2,s_pc3, s_CL3, t_G )
                )
               &(
                  ( programInt1_2(t_pc2,s_pc2,t_CL2,s_CL2,t_G,s_G)      // Assignment related
                    & CopyVariables_ProgramInt_2(t_pc2,t_CL2,s_CL2,t_G,s_G)   //  Copy others global variable
                  )
                  | programInt3(2, t_pc2, s_pc2, t_CL2, s_CL2, t_G, s_G )   // constrain
                )
           )
      )
    )

  | (
         true
       & (exists PrCount t_pc2.
             (
                  Thread2_1_Forward_AbsLocalKeepG( s_pc1, s_CL1, t_pc2, s_CL2,s_pc3, s_CL3, s_G )
                 & programInt2_2(t_pc2,s_pc2,s_CL2,s_G)    // Control flow statement
             )
         )
    )

);
#size Thread2_1_Forward_AbsLocalKeepG;
#timer;

// ###   #  #  ###   ####   ##   ###         ####
//  #    #  #  #  #  #     #  #  #  #           #
//  #    ####  #  #  ###   #  #  #  #         ##
//  #    #  #  ###   #     ####  #  #           #
//  #    #  #  # #   #     #  #  #  #        #  #
//  #    #  #  #  #  ####  #  #  ###          ##

mu bool Thread3_1_Forward_AbsLocalKeepG(
 PrCount    s_pc1,                  // Program counter
 Local      s_CL1,                  // Local variable
 PrCount    s_pc2,                  // Program counter
 Local      s_CL2,                  // Local variable
 PrCount    s_pc3,                  // Program counter
 Local      s_CL3,                  // Local variable
 Global     s_G                      // Global variable
)
 s_pc1    <  s_CL1,
 s_CL1    <  s_pc2,
 s_pc2    <  s_CL2,
 s_CL2    <  s_pc3,
 s_pc3    <  s_CL3,
 s_CL3    <  s_G
( false
  // initial configuration (init)
  |  (
        (exists
          Local   t_CL1,
          Local   t_CL2,
          Local   t_CL3.
          (
                 Thread2_1_Forward_AbsLocalKeepG( s_pc1, t_CL1, s_pc2, t_CL2, s_pc3, t_CL3, s_G )
              &  Thread3_1_Backward_AbsLocal( s_pc1, t_CL1, s_pc2, t_CL2, s_pc3, t_CL3, s_G )
          )
        )
              &  s_G.v1 = 0
        &  ContextSwitch_2(s_pc2)
     )

//*********************************************************************************/
// forward propagation on internal transitions
//*********************************************************************************/

  |  (
      true
      & (exists                  // There exists an internal state that
           PrCount t_pc3,
           Local   t_CL3,
           Global  t_G.
           (    (
                   Thread3_1_Forward_AbsLocalKeepG( s_pc1, s_CL1, s_pc2, s_CL2, t_pc3, t_CL3, t_G )
                )
               &(
                  ( programInt1_3(t_pc3,s_pc3,t_CL3,s_CL3,t_G,s_G)      // Assignment related
                    & CopyVariables_ProgramInt_3(t_pc3,t_CL3,s_CL3,t_G,s_G)   //  Copy others global variable
                  )
                  | programInt3(3, t_pc3, s_pc3, t_CL3, s_CL3, t_G, s_G )   // constrain
                )
           )
      )
    )

  | (
         true
       & (exists PrCount t_pc3.
             (
              Thread3_1_Forward_AbsLocalKeepG( s_pc1, s_CL1, s_pc2, s_CL2,t_pc3, s_CL3, s_G )
                 & programInt2_3(t_pc3,s_pc3,s_CL3,s_G)    // Control flow statement
             )
         )
    )

);
#size Thread3_1_Forward_AbsLocalKeepG;
#timer;



// ####    ###   #   #  #   #  ####           ###
// #   #  #   #  #   #  #   #   #  #         #   #
// #   #  #   #  #   #  ##  #   #  #             #
// ####   #   #  #   #  # # #   #  #           ##
// # #    #   #  #   #  #  ##   #  #          #
// #  #   #   #  #   #  #   #   #  #         #
// #   #   ###    ###   #   #  ####          #####

// FORWARD ABSTRACTION


// ###   #  #  ###   ####   ##   ###          #
//  #    #  #  #  #  #     #  #  #  #        ##
//  #    ####  #  #  ###   #  #  #  #         #
//  #    #  #  ###   #     ####  #  #         #
//  #    #  #  # #   #     #  #  #  #         #
//  #    #  #  #  #  ####  #  #  ###         ###

mu bool Thread1_2_Forward_AbsLocalKeepG(
 PrCount    s_pc1,                  // Program counter
 Local      s_CL1,                  // Local variable
 PrCount    s_pc2,                  // Program counter
 Local      s_CL2,                  // Local variable
 PrCount    s_pc3,                  // Program counter
 Local      s_CL3,                  // Local variable
 Global     s_G                    // Global variable
)
 s_pc1    <  s_CL1,
 s_CL1    <  s_pc2,
 s_pc2    <  s_CL2,
 s_CL2    <  s_pc3,
 s_pc3    <  s_CL3,
 s_CL3    <  s_G
( false

  // initial configuration (init)
  |  (
        (exists
          Local   t_CL1,
          Local   t_CL2,
          Local   t_CL3.
          (
                 Thread3_1_Forward_AbsLocalKeepG( s_pc1, t_CL1, s_pc2, t_CL2, s_pc3, t_CL3, s_G )
              &  Thread1_2_Backward_AbsLocal( s_pc1, t_CL1, s_pc2, t_CL2, s_pc3, t_CL3, s_G )
          )
        )
              &  s_G.v1 = 0
        &  ContextSwitch_3(s_pc3)
     )

//*********************************************************************************/
// forward propagation on internal transitions
//*********************************************************************************/

  |  (
         true
      & (exists                  // There exists an internal state that
           PrCount t_pc1,
           Local   t_CL1,
           Global t_G.
           (    (   Thread1_2_Forward_AbsLocalKeepG( t_pc1, t_CL1, s_pc2, s_CL2,s_pc3, s_CL3, t_G )
                )
               &(
                  ( programInt1_1(t_pc1,s_pc1,t_CL1,s_CL1,t_G,s_G)      // Assignment related
                    & CopyVariables_ProgramInt_1(t_pc1,t_CL1,s_CL1,t_G,s_G)   //  Copy others global variable
                  )
                  | programInt3(1, t_pc1, s_pc1, t_CL1, s_CL1, t_G, s_G )   // constrain
                )
           )
      )
    )

  | (
         true
       & (exists PrCount t_pc1.
             (     Thread1_2_Forward_AbsLocalKeepG( t_pc1, s_CL1, s_pc2, s_CL2,s_pc3, s_CL3, s_G )
                 & programInt2_1(t_pc1,s_pc1,s_CL1,s_G)    // Control flow statement
             )
         )
    )

);
#size Thread1_2_Forward_AbsLocalKeepG;
#timer;

 // #####  #   #  ####   #####    #    ####          ###
 //   #    #   #  #   #  #       # #    #  #        #   #
 //   #    #   #  #   #  #      #   #   #  #            #
 //   #    #####  ####   ####   #   #   #  #          ##
 //   #    #   #  # #    #      #####   #  #         #
 //   #    #   #  #  #   #      #   #   #  #        #
 //   #    #   #  #   #  #####  #   #  ####         #####

mu bool Thread2_2_Forward_AbsLocalKeepG(
 PrCount    s_pc1,                  // Program counter
 Local      s_CL1,                  // Local variable
 PrCount    s_pc2,                  // Program counter
 Local      s_CL2,                  // Local variable
 PrCount    s_pc3,                  // Program counter
 Local      s_CL3,                  // Local variable
 Global     s_G                    // Global variable
)
 s_pc1    <  s_CL1,
 s_CL1    <  s_pc2,
 s_pc2    <  s_CL2,
 s_CL2    <  s_pc3,
 s_pc3    <  s_CL3,
 s_CL3    <  s_G
( false

  // initial configuration (init)
  |  (
        (exists
          Local   t_CL1,
          Local   t_CL2,
          Local   t_CL3.
          (
                 Thread1_2_Forward_AbsLocalKeepG( s_pc1, t_CL1, s_pc2, t_CL2, s_pc3, t_CL3, s_G )
              &  Thread2_2_Backward_AbsLocal( s_pc1, t_CL1, s_pc2, t_CL2, s_pc3, t_CL3, s_G )
          )
        )
        &  s_G.v1 = 0
        &  ContextSwitch_1(s_pc1)
     )

//*********************************************************************************/
// forward propagation on internal transitions
//*********************************************************************************/

  |  (
      true
      & (exists                  // There exists an internal state that
           PrCount t_pc2,
           Local   t_CL2,
           Global  t_G.
           (    (   Thread2_2_Forward_AbsLocalKeepG( s_pc1, s_CL1, t_pc2, t_CL2,s_pc3, s_CL3, t_G )
                )
               &(
                  ( programInt1_2(t_pc2,s_pc2,t_CL2,s_CL2,t_G,s_G)      // Assignment related
                    & CopyVariables_ProgramInt_2(t_pc2,t_CL2,s_CL2,t_G,s_G)   //  Copy others global variable
                  )
                  | programInt3(2, t_pc2, s_pc2, t_CL2, s_CL2, t_G, s_G )   // constrain
                )
           )
      )
    )

  | (
         true
       & (exists PrCount t_pc2.
             (     Thread2_2_Forward_AbsLocalKeepG( s_pc1, s_CL1, t_pc2, s_CL2,s_pc3, s_CL3, s_G )
                 & programInt2_2(t_pc2,s_pc2,s_CL2,s_G)    // Control flow statement
             )
         )
    )

);
#size Thread2_2_Forward_AbsLocalKeepG;
#timer;

// ###   #  #  ###   ####   ##   ###         ####
//  #    #  #  #  #  #     #  #  #  #           #
//  #    ####  #  #  ###   #  #  #  #         ##
//  #    #  #  ###   #     ####  #  #           #
//  #    #  #  # #   #     #  #  #  #        #  #
//  #    #  #  #  #  ####  #  #  ###          ##

mu bool Thread3_2_Forward_AbsLocalKeepG(
 PrCount    s_pc1,                  // Program counter
 Local      s_CL1,                  // Local variable
 PrCount    s_pc2,                  // Program counter
 Local      s_CL2,                  // Local variable
 PrCount    s_pc3,                  // Program counter
 Local      s_CL3,                  // Local variable
 Global     s_G                      // Global variable
)
 s_pc1    <  s_CL1,
 s_CL1    <  s_pc2,
 s_pc2    <  s_CL2,
 s_CL2    <  s_pc3,
 s_pc3    <  s_CL3,
 s_CL3    <  s_G
( false
  // initial configuration (init)
  |  (
        (exists
          Local   t_CL1,
          Local   t_CL2,
          Local   t_CL3.
          (
                 Thread2_2_Forward_AbsLocalKeepG( s_pc1, t_CL1, s_pc2, t_CL2, s_pc3, t_CL3, s_G )
              &  Thread3_2_Backward_AbsLocal( s_pc1, t_CL1, s_pc2, t_CL2, s_pc3, t_CL3, s_G )
          )
        )
        &  s_G.v1 = 0
        &  ContextSwitch_2(s_pc2)
     )

//*********************************************************************************/
// forward propagation on internal transitions
//*********************************************************************************/

  |  (
      true
      & (exists                  // There exists an internal state that
           PrCount t_pc3,
           Local   t_CL3,
           Global  t_G.
           (    (   Thread3_2_Forward_AbsLocalKeepG( s_pc1, s_CL1, s_pc2, s_CL2, t_pc3, t_CL3, t_G )
                )
               &(
                  ( programInt1_3(t_pc3,s_pc3,t_CL3,s_CL3,t_G,s_G)      // Assignment related
                    & CopyVariables_ProgramInt_3(t_pc3,t_CL3,s_CL3,t_G,s_G)   //  Copy others global variable
                  )
                  | programInt3(3, t_pc3, s_pc3, t_CL3, s_CL3, t_G, s_G )   // constrain
                )
           )
      )
    )

  | (
         true
       & (exists PrCount t_pc3.
             (     Thread3_2_Forward_AbsLocalKeepG( s_pc1, s_CL1, s_pc2, s_CL2,t_pc3, s_CL3, s_G )
                 & programInt2_3(t_pc3,s_pc3,s_CL3,s_G)    // Control flow statement
             )
         )
    )

);
#size Thread3_2_Forward_AbsLocalKeepG;
#timer;


// ####    ###   #   #  #   #  ####          #####
// #   #  #   #  #   #  #   #   #  #             #
// #   #  #   #  #   #  ##  #   #  #            #
// ####   #   #  #   #  # # #   #  #           ##
// # #    #   #  #   #  #  ##   #  #             #
// #  #   #   #  #   #  #   #   #  #         #   #
// #   #   ###    ###   #   #  ####           ###

// FORWARD ABSTRACTION

// ###   #  #  ###   ####   ##   ###          #
//  #    #  #  #  #  #     #  #  #  #        ##
//  #    ####  #  #  ###   #  #  #  #         #
//  #    #  #  ###   #     ####  #  #         #
//  #    #  #  # #   #     #  #  #  #         #
//  #    #  #  #  #  ####  #  #  ###         ###

mu bool Thread1_3_Forward_AbsLocalKeepG(
 PrCount    s_pc1,                  // Program counter
 Local      s_CL1,                  // Local variable
 PrCount    s_pc2,                  // Program counter
 Local      s_CL2,                  // Local variable
 PrCount    s_pc3,                  // Program counter
 Local      s_CL3,                  // Local variable
 Global     s_G                    // Global variable
)
 s_pc1    <  s_CL1,
 s_CL1    <  s_pc2,
 s_pc2    <  s_CL2,
 s_CL2    <  s_pc3,
 s_pc3    <  s_CL3,
 s_CL3    <  s_G
( false

  // initial configuration (init)
  |  (
        (exists
          Local   t_CL1,
          Local   t_CL2,
          Local   t_CL3.
          (
                 Thread3_2_Forward_AbsLocalKeepG( s_pc1, t_CL1, s_pc2, t_CL2, s_pc3, t_CL3, s_G )
              &  Thread1_3_Backward_AbsLocal( s_pc1, t_CL1, s_pc2, t_CL2, s_pc3, t_CL3, s_G )
          )
        )
              &  s_G.v1 = 0
        &  ContextSwitch_3(s_pc3)
     )

//*********************************************************************************/
// forward propagation on internal transitions
//*********************************************************************************/

  |  (
         true
      & (exists                  // There exists an internal state that
           PrCount t_pc1,
           Local   t_CL1,
           Global t_G.
           (    (   Thread1_3_Forward_AbsLocalKeepG( t_pc1, t_CL1, s_pc2, s_CL2,s_pc3, s_CL3, t_G )
                )
               &(
                  ( programInt1_1(t_pc1,s_pc1,t_CL1,s_CL1,t_G,s_G)      // Assignment related
                    & CopyVariables_ProgramInt_1(t_pc1,t_CL1,s_CL1,t_G,s_G)   //  Copy others global variable
                  )
                  | programInt3(1, t_pc1, s_pc1, t_CL1, s_CL1, t_G, s_G )   // constrain
                )
           )
      )
    )

  | (
         true
       & (exists PrCount t_pc1.
             (     Thread1_3_Forward_AbsLocalKeepG( t_pc1, s_CL1, s_pc2, s_CL2,s_pc3, s_CL3, s_G )
                 & programInt2_1(t_pc1,s_pc1,s_CL1,s_G)    // Control flow statement
             )
         )
    )

);
#size Thread1_3_Forward_AbsLocalKeepG;
#timer;

 // #####  #   #  ####   #####    #    ####          ###
 //   #    #   #  #   #  #       # #    #  #        #   #
 //   #    #   #  #   #  #      #   #   #  #            #
 //   #    #####  ####   ####   #   #   #  #          ##
 //   #    #   #  # #    #      #####   #  #         #
 //   #    #   #  #  #   #      #   #   #  #        #
 //   #    #   #  #   #  #####  #   #  ####         #####

mu bool Thread2_3_Forward_AbsLocalKeepG(
 PrCount    s_pc1,                  // Program counter
 Local      s_CL1,                  // Local variable
 PrCount    s_pc2,                  // Program counter
 Local      s_CL2,                  // Local variable
 PrCount    s_pc3,                  // Program counter
 Local      s_CL3,                  // Local variable
 Global     s_G                    // Global variable
)
 s_pc1    <  s_CL1,
 s_CL1    <  s_pc2,
 s_pc2    <  s_CL2,
 s_CL2    <  s_pc3,
 s_pc3    <  s_CL3,
 s_CL3    <  s_G
( false

  // initial configuration (init)
  |  (
        (exists
          Local   t_CL1,
          Local   t_CL2,
          Local   t_CL3.
          (
                 Thread1_3_Forward_AbsLocalKeepG( s_pc1, t_CL1, s_pc2, t_CL2, s_pc3, t_CL3, s_G )
              &  Thread2_3_Backward_AbsLocal( s_pc1, t_CL1, s_pc2, t_CL2, s_pc3, t_CL3, s_G )
          )
        )
              &  s_G.v1 = 0
        &  ContextSwitch_1(s_pc1)
     )

//*********************************************************************************/
// forward propagation on internal transitions
//*********************************************************************************/

  |  (
      true
      & (exists                  // There exists an internal state that
           PrCount t_pc2,
           Local   t_CL2,
           Global  t_G.
           (    (   Thread2_3_Forward_AbsLocalKeepG( s_pc1, s_CL1, t_pc2, t_CL2,s_pc3, s_CL3, t_G )
                )
               &(
                  ( programInt1_2(t_pc2,s_pc2,t_CL2,s_CL2,t_G,s_G)      // Assignment related
                    & CopyVariables_ProgramInt_2(t_pc2,t_CL2,s_CL2,t_G,s_G)   //  Copy others global variable
                  )
                  | programInt3(2, t_pc2, s_pc2, t_CL2, s_CL2, t_G, s_G )   // constrain
                )
           )
      )
    )

  | (
         true
       & (exists PrCount t_pc2.
             (     Thread2_3_Forward_AbsLocalKeepG( s_pc1, s_CL1, t_pc2, s_CL2,s_pc3, s_CL3, s_G )
                 & programInt2_2(t_pc2,s_pc2,s_CL2,s_G)    // Control flow statement
             )
         )
    )

);
#size Thread2_3_Forward_AbsLocalKeepG;
#timer;

// ###   #  #  ###   ####   ##   ###         ####
//  #    #  #  #  #  #     #  #  #  #           #
//  #    ####  #  #  ###   #  #  #  #         ##
//  #    #  #  ###   #     ####  #  #           #
//  #    #  #  # #   #     #  #  #  #        #  #
//  #    #  #  #  #  ####  #  #  ###          ##

mu bool Thread3_3_Forward_AbsLocalKeepG(
 PrCount    s_pc1,                  // Program counter
 Local      s_CL1,                  // Local variable
 PrCount    s_pc2,                  // Program counter
 Local      s_CL2,                  // Local variable
 PrCount    s_pc3,                  // Program counter
 Local      s_CL3,                  // Local variable
 Global     s_G                      // Global variable
)
 s_pc1    <  s_CL1,
 s_CL1    <  s_pc2,
 s_pc2    <  s_CL2,
 s_CL2    <  s_pc3,
 s_pc3    <  s_CL3,
 s_CL3    <  s_G
( false
  // initial configuration (init)
  |  (
        (exists
          Local   t_CL1,
          Local   t_CL2,
          Local   t_CL3.
          (
                 Thread2_3_Forward_AbsLocalKeepG( s_pc1, t_CL1, s_pc2, t_CL2, s_pc3, t_CL3, s_G )
              &  Thread3_3_Backward_AbsLocal( s_pc1, t_CL1, s_pc2, t_CL2, s_pc3, t_CL3, s_G )
          )
        )
              &  s_G.v1 = 0
        &  ContextSwitch_2(s_pc2)
     )

//*********************************************************************************/
// forward propagation on internal transitions
//*********************************************************************************/

  |  (
      true
      & (exists                  // There exists an internal state that
           PrCount t_pc3,
           Local   t_CL3,
           Global  t_G.
           (    (   Thread3_3_Forward_AbsLocalKeepG( s_pc1, s_CL1, s_pc2, s_CL2, t_pc3, t_CL3, t_G )
                )
               &(
                  ( programInt1_3(t_pc3,s_pc3,t_CL3,s_CL3,t_G,s_G)      // Assignment related
                    & CopyVariables_ProgramInt_3(t_pc3,t_CL3,s_CL3,t_G,s_G)   //  Copy others global variable
                  )
                  | programInt3(3, t_pc3, s_pc3, t_CL3, s_CL3, t_G, s_G )   // constrain
                )
           )
      )
    )

  | (
         true
       & (exists PrCount t_pc3.
             (     Thread3_3_Forward_AbsLocalKeepG( s_pc1, s_CL1, s_pc2, s_CL2,t_pc3, s_CL3, s_G )
                 & programInt2_3(t_pc3,s_pc3,s_CL3,s_G)    // Control flow statement
             )
         )
    )

);
#size Thread3_3_Forward_AbsLocalKeepG;
#timer;

