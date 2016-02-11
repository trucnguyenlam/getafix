/*************************************************************************************************/
/********************                 DESIGN FOR 3 THREADS            ****************************/
/*************************************************************************************************/
#timer go;

enum blocktype {             // INIT is mod=0
                thread1,     // starting from T1
                thread2,
                thread3,
                thread1noloc,
                thread2noloc,
                thread3noloc,
                have23,       // Have block (simply mean RLI)
                have3       // Have block (simply mean RLI)
              };


/*************************************************************************************************/
/*************               Used in internal transition                 *************************/
/*************************************************************************************************/


/*
  Copy global variable at CS 3 (mean round 3)
 */
bool copy_g_and_h_3( Globals s_G, Globals t_G)
 s_G ~+ t_G
(   true
    & s_G.g0 =t_G.g0 & s_G.h0 =t_G.h0
    & s_G.g1 =t_G.g1 & s_G.h1 =t_G.h1
    & s_G.g2 =t_G.g2 & s_G.h2 =t_G.h2
    & s_G.g3 =t_G.g3
);

/*
  Copy global variable at CS 2 (mean round 2)
 */
bool copy_g_and_h_2( Globals s_G, Globals t_G)
 s_G ~+ t_G
(   true
    & s_G.g0 =t_G.g0 & s_G.h0 =t_G.h0
    & s_G.g1 =t_G.g1 & s_G.h1 =t_G.h1
    & s_G.g2 =t_G.g2
    & s_G.g3 =t_G.g3 & s_G.h3 =t_G.h3
);


/*
  Copy global variable at CS 1 (mean round 1)
 */
bool copy_g_and_h_1( Globals s_G, Globals t_G)
 s_G ~+ t_G
(   true
    & s_G.g0 =t_G.g0 & s_G.h0 =t_G.h0
    & s_G.g1 =t_G.g1
    & s_G.g2 =t_G.g2 & s_G.h2 =t_G.h2
    & s_G.g3 =t_G.g3 & s_G.h3 =t_G.h3
);


/*
  Copy global variable at CS 0 (mean round 0)
 */
bool copy_g_and_h_0( Globals s_G, Globals t_G)
 s_G ~+ t_G
(   true
    & s_G.g0 =t_G.g0
    & s_G.g1 =t_G.g1 & s_G.h1 =t_G.h1
    & s_G.g2 =t_G.g2 & s_G.h2 =t_G.h2
    & s_G.g3 =t_G.g3 & s_G.h3 =t_G.h3
);



/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/******                                                                                    *******/
/******                               Reachability Algorithm                               *******/
/******                                                                                    *******/
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/


mu bool Init_Reach(
 PrCount s_pc,      // Program counter
 Local   s_CL,      // local variable
 Global  s_G        // global variable
)
 s_pc    <  s_CL,
 s_CL    <  s_G
(
  false

  // initial conf

  | initPC(s_pc)    // simply mean init PC

  // forward propagation on internal transitions
  |  ( exists
           PrCount t_pc,
           Local   t_CL,
           Global  t_G.
           (   Init_Reach( t_pc, t_CL, t_G )
               &(t_G.v1=0 | ( t_G.v1=1 & t_CL.v0=1 ) )
               &( ( programInt1( 0, t_pc, s_pc, t_CL, s_CL, t_G, s_G )    // Init module
                    & CopyVariables_ProgramInt( 2, t_pc, t_CL, s_CL, t_G, s_G )
                  )
                  | programInt3( 0, t_pc, s_pc, t_CL, s_CL, t_G, s_G )      // Init module
                )
           )
     )

  | ( exists PrCount t_pc.
           (     Init_Reach( t_pc, s_CL, s_G )
               &(s_G.v1=0 | ( s_G.v1=1 & s_CL.v0=1 ) )
               & programInt2( 0, t_pc, s_pc, s_CL, s_G )                // Transitions that only use control flow
           )
      )

);
#size Init_Reach;     //? Show the size of formula
#timer;


// ###   #  #  ###   ###
//  #    ## #   #     #
//  #    ## #   #     #
//  #    # ##   #     #
//  #    # ##   #     #
// ###   #  #  ###    #



bool GlobalInit(Global CG)           // Call Init on Global (shared) variable
( exists
         Module  s_mod,
         PrCount s_pc,
         Local   s_CL.
              (   Init_Reach( s_pc, s_CL, CG )
                & Target_Init(s_pc )
              )
);



 // ####    ###   #   #  #   #  ####            #
 // #   #  #   #  #   #  #   #   #  #          # #
 // #   #  #   #  #   #  ##  #   #  #         #   #
 // ####   #   #  #   #  # # #   #  #         #   #
 // # #    #   #  #   #  #  ##   #  #         #   #
 // #  #   #   #  #   #  #   #   #  #          # #
 // #   #   ###    ###   #   #  ####            #



// ###   #  #  ###   ####   ##   ###          #
//  #    #  #  #  #  #     #  #  #  #        ##
//  #    ####  #  #  ###   #  #  #  #         #
//  #    #  #  ###   #     ####  #  #         #
//  #    #  #  # #   #     #  #  #  #         #
//  #    #  #  #  #  ####  #  #  ###         ###

mu bool Thread1_0(
 blocktype  s_block,               // Block as appeared in Figure 2 of cav2010
 Module     s_mod,                 // Current module (thread)
 PrCount    s_pc,                  // Program counter
 Local      s_CL,                  // Local variable
 Globals    s_G                    // Global variable
)
 s_block <  s_mod,
 s_mod   <  s_pc,
 s_pc    <  s_CL,
 s_CL    <  s_G
( false

  // early termination
  | ( exists                   // There exists a state such that
            blocktype  t_block,
            Module     t_mod,
            PrCount    t_pc,
            Local      t_CL,
            Globals    t_G.
        (   Thread1_0( t_block, t_mod, t_pc, t_CL, t_G )    // That state in fixed point and ...
          & (   target( t_mod, t_pc )                // target is reached
              & ( false
                  |  t_block=thread1
                )
            )
        )
     )

  /*********************************************************************************/
  // thread1 (first thread) (TLI)

  // initial configuration (init)
  |  (
         s_block = thread1              // Start thread1 if
         & initPC(s_pc)                 // Initial PC
         & s_mod=1                      // In module 1(thread1)
         & GlobalInit(s_G.g0)           // INIT runs FIRST
         // At this state, input = output
         & s_G.h0=s_G.g0
         // & ( forall PrCount t_pc. (!target( 0, t_pc)))  // ??????????????/**/
     )

/*
                                    |------||------|
                                    | T1   ||  H23 |
                                    |      ||      |
                                    | s_G  || a_G  |
                                    |      ||      |
                              t_r:  |      ||------|
                                    |      ||
                              s_r:  |------||
*/

//*********************************************************************************/
// forward propagation on internal transitions
//*********************************************************************************/

//*************** Round 0
  |  (
        (
          s_G.g0.v1 =0     // Why throw in                                atomic=F?
          & (s_block=thread1)   // Throw in any thread
        )
      & (exists                  // There exists an internal state that
           PrCount t_pc,
           Local   t_CL,
           Globals t_G.
           (    (   Thread1_0( s_block, s_mod, t_pc, t_CL, t_G )
                  & copy_g_and_h_0(s_G,t_G)    // state of global unchange
                )
               &( t_G.h0.v1=0 | ( t_G.h0.v1=1 & t_CL.v0=1 ) )   // Start, so when it ends?
               &(
                  ( programInt1(s_mod,t_pc,s_pc,t_CL,s_CL,t_G.h0,s_G.h0)      // Assignment related
                    & CopyVariables_ProgramInt(s_mod,t_pc,t_CL,s_CL,t_G.h0,s_G.h0)   //  Copy others global variable
                  )
                  | programInt3( s_mod, t_pc, s_pc, t_CL, s_CL, t_G.h0, s_G.h0 )   // constrain
                )
           )
      )
    )

  | (
        (
          s_G.g0.v1 =0
          & (s_block=thread1)
        )
       & (s_G.h0.v1=0 | ( s_G.h0.v1=1 & s_CL.v0=1 ) )     // Atomic condition
       & (exists PrCount t_pc.
             (     Thread1_0( s_block, s_mod, t_pc, s_CL, s_G )
                 & programInt2(s_mod,t_pc,s_pc,s_CL,s_G.h0)    // Control flow statement
             )
         )
    )

);
#size Thread1_0;
#timer;

bool ForgetLoc_r0_t1(     // Round 0 thread 1
 Globals    s_G                    // Global variable
)
(
   // forgetting local states for  each thread
  ( exists blocktype  t_block,
       Module     t_mod,
       PrCount    t_pc,
       Local      t_CL,
       Globals    t_G.
       (    Thread1_0( t_block, t_mod, t_pc, t_CL, t_G )
          & ( false
              | (t_block=thread1)
            )
          & copy_g_g( t_G, s_G, 0 )
          & copy_h_h( t_G, s_G, 0 )
          & (
                t_G.h0.v1=0   // Atomic:=F
            )
       )
    )
);
#size ForgetLoc_r0_t1;
#timer;



 // #####  #   #  ####   #####    #    ####          ###
 //   #    #   #  #   #  #       # #    #  #        #   #
 //   #    #   #  #   #  #      #   #   #  #            #
 //   #    #####  ####   ####   #   #   #  #          ##
 //   #    #   #  # #    #      #####   #  #         #
 //   #    #   #  #  #   #      #   #   #  #        #
 //   #    #   #  #   #  #####  #   #  ####         #####

mu bool Thread2_0(
 blocktype  s_block,               // Block as appeared in Figure 2 of cav2010
 Module     s_mod,                 // Current module (thread)
 PrCount    s_pc,                  // Program counter
 Local      s_CL,                  // Local variable
 Globals    s_G                    // Global variable
)
 s_block <  s_mod,
 s_mod   <  s_pc,
 s_pc    <  s_CL,
 s_CL    <  s_G
( false

  // early termination

  | ( exists                   // There exists a state such that
            blocktype  t_block,
            Module     t_mod,
            PrCount    t_pc,
            Local      t_CL,
            Globals    t_G.
        (   Thread2_0( t_block, t_mod, t_pc, t_CL, t_G )    // That state in fixed point and ...
          & (   target( t_mod, t_pc )                // target is reached
              & ( false
                  |  t_block=thread2
                )
            )
        )
     )

  /*********************************************************************************/
  // thread2

  // initial configuration
  |  (   s_block=thread2     // This is thread2 if
         & initPC(s_pc)      // begin with initial pc
         & s_mod=2           // in module 2 (thread2)
         & ( exists  Globals t_G.
              (
                ForgetLoc_r0_t1(t_G)
                & t_G.h0 = s_G.g0    // Map output of thread1 to input of thread2
              )
           )
         // At this state, input = output
         & s_G.h0=s_G.g0
     )


/*
                                     ------------------
                                    |  ------  ------  |
                                    | | T2   ||  H3  | |
                                    | |      ||      | |
                                    | | s_G  || a_G  | |
                                    | |      ||      | |
                                    |  ------  ------  |
                                    |        b_G    W23|
                                     -----------------
*/


//*********************************************************************************/
// forward propagation on internal transitions
//*********************************************************************************/


//*************** Round 0
  |  (
        (
          s_G.g0.v1 =0     // Why throw in                                atomic=F?
          & (s_block=thread2)   // Throw in any thread
        )
      & (exists                  // There exists an internal state that
           PrCount t_pc,
           Local   t_CL,
           Globals t_G.
           (    (
                    Thread2_0( s_block, s_mod, t_pc, t_CL, t_G )
                  & copy_g_and_h_0(s_G,t_G)    // state of global unchange
                )
               &( t_G.h0.v1=0 | ( t_G.h0.v1=1 & t_CL.v0=1 ) )   // Start, so when it ends?
               &(
                  ( programInt1(s_mod,t_pc,s_pc,t_CL,s_CL,t_G.h0,s_G.h0)      // Assignment related
                    & CopyVariables_ProgramInt(s_mod,t_pc,t_CL,s_CL,t_G.h0,s_G.h0)   //  Copy others global variable
                  )
                  | programInt3( s_mod, t_pc, s_pc, t_CL, s_CL, t_G.h0, s_G.h0 )   // constrain
                )
           )
      )
    )


  | (
        (
          s_G.g0.v1 =0
          & (s_block=thread2)
        )
       & (s_G.h0.v1=0 | ( s_G.h0.v1=1 & s_CL.v0=1 ) )     // Atomic condition
       & (exists PrCount t_pc.
             (
                Thread2_0( s_block, s_mod, t_pc, s_CL, s_G )
                 & programInt2(s_mod,t_pc,s_pc,s_CL,s_G.h0)    // Control flow statement
             )
         )
    )

);

#size Thread2_0;
#timer;


bool ForgetLoc_r0_t2(     // Round 0 thread 2
 Globals    s_G                    // Global variable
)
(
   // forgetting local states for  each thread
  ( exists blocktype  t_block,
             Module     t_mod,
             PrCount    t_pc,
             Local      t_CL,
             Globals    t_G.
             (    Thread2_0( t_block, t_mod, t_pc, t_CL, t_G )
                & ( false
                    | (t_block=thread2)
                  )
                & copy_g_g( t_G, s_G, 0 )
                & copy_h_h( t_G, s_G, 0 )
                & (
                      t_G.h0.v1=0   // Atomic:=F
                  )
             )
    )
);
#size ForgetLoc_r0_t2;
#timer;


// ###   #  #  ###   ####   ##   ###         ####
//  #    #  #  #  #  #     #  #  #  #           #
//  #    ####  #  #  ###   #  #  #  #         ##
//  #    #  #  ###   #     ####  #  #           #
//  #    #  #  # #   #     #  #  #  #        #  #
//  #    #  #  #  #  ####  #  #  ###          ##

mu bool Thread3_0(
 blocktype  s_block,               // Block as appeared in Figure 2 of cav2010
 Module     s_mod,                 // Current module (thread)
 PrCount    s_pc,                  // Program counter
 Local      s_CL,                  // Local variable
 Globals    s_G                    // Global variable
)
 s_block <  s_mod,
 s_mod   <  s_pc,
 s_pc    <  s_CL,
 s_CL    <  s_G
( false

  // early termination

  | ( exists                   // There exists a state such that
            blocktype  t_block,
            Module     t_mod,
            PrCount    t_pc,
            Local      t_CL,
            Globals    t_G.
        (   Thread3_0( t_block, t_mod, t_pc, t_CL, t_G )    // That state in fixed point and ...
          & (   target( t_mod, t_pc )                // target is reached
              & ( false
                  |  t_block=thread3
                )
            )
        )
     )



  /*********************************************************************************/
  // thread3

  // initial configuration
  |  (   s_block=thread3     // This is thread3 if
         & initPC(s_pc)      // begin with initial pc
         & s_mod=3           // in module 2 (thread3)
         & ( exists  Globals t_G.
              (
                ForgetLoc_r0_t2(t_G)
                & t_G.h0 = s_G.g0    // Map output of thread1 to input of thread3
              )
           )
         // At this state, input = output
         & s_G.h0=s_G.g0
     )


/*
                                     ------------------
                                    |  ------  ------  |
                                    | | T2   ||  H3  | |
                                    | |      ||      | |
                                    | | s_G  || a_G  | |
                                    | |      ||      | |
                                    |  ------  ------  |
                                    |        b_G    W23|
                                     -----------------
*/


//*********************************************************************************/
// forward propagation on internal transitions
//*********************************************************************************/


//*************** Round 0
  |  (
        (
          s_G.g0.v1 =0     // Why throw in                                atomic=F?
          & (s_block=thread3)   // Throw in any thread
        )
      & (exists                  // There exists an internal state that
           PrCount t_pc,
           Local   t_CL,
           Globals t_G.
           (    (
                    Thread3_0( s_block, s_mod, t_pc, t_CL, t_G )
                  & copy_g_and_h_0(s_G,t_G)    // state of global unchange
                )
               &( t_G.h0.v1=0 | ( t_G.h0.v1=1 & t_CL.v0=1 ) )   // Start, so when it ends?
               &(
                  ( programInt1(s_mod,t_pc,s_pc,t_CL,s_CL,t_G.h0,s_G.h0)      // Assignment related
                    & CopyVariables_ProgramInt(s_mod,t_pc,t_CL,s_CL,t_G.h0,s_G.h0)   //  Copy others global variable
                  )
                  | programInt3( s_mod, t_pc, s_pc, t_CL, s_CL, t_G.h0, s_G.h0 )   // constrain
                )
           )
      )
    )

  | (
        (
          s_G.g0.v1 =0
          & (s_block=thread3)
        )
       & (s_G.h0.v1=0 | ( s_G.h0.v1=1 & s_CL.v0=1 ) )     // Atomic condition
       & (exists PrCount t_pc.
             (
                  Thread3_0( s_block, s_mod, t_pc, s_CL, s_G )
                & programInt2(s_mod,t_pc,s_pc,s_CL,s_G.h0)    // Control flow statement
             )
         )
    )

);
#size Thread3_0;
#timer;

bool ForgetLoc_r0_t3(     // Round 0 thread 3
 Globals    s_G                    // Global variable
)
(
   // forgetting local states for  each thread
  ( exists blocktype  t_block,
             Module     t_mod,
             PrCount    t_pc,
             Local      t_CL,
             Globals    t_G.
             (    Thread3_0( t_block, t_mod, t_pc, t_CL, t_G )
                & ( false
                    | (t_block=thread3)
                  )
                & copy_g_g( t_G, s_G, 0 )
                & copy_h_h( t_G, s_G, 0 )
                & (
                      t_G.h0.v1=0   // Atomic:=F
                  )
             )
    )
);
#size ForgetLoc_r0_t3;
#timer;


// #  #   ##   #  #  ####               #
// #  #  #  #  #  #  #                 # #
// ####  #  #  #  #  ###               # #
// #  #  ####  #  #  #                 # #
// #  #  #  #   ##   #                 # #
// #  #  #  #   ##   ####               #


mu bool Have0(    // Have block at round 0
 blocktype  s_block,
 Globals    s_G                    // Global variable
)
s_block < s_G
( false

  /*********************************************************************************/
  // have block (backward phase)

  // have blocks generated from the thread3 (stop forward propagation)
  | (    s_block=have3                  // This block will become have block (RLI if
         & ForgetLoc_r0_t3(s_G)
    )


/*
                                    |------|        |------|
                                    | T3   |        | T3   |
                                    |      |        |      |
                                    | s_G  | become | s_G  |
                                    |      |  -->   |      |
                                    |      |        |      |
                                    |      |        |      |
                                    |------|        |------|
                                        W3             H3
*/

  // have blocks generated by thread2
  | (    s_block=have23
         & ( exists Globals b_G,
                    Globals a_G.
                (
                    ForgetLoc_r0_t2(a_G)
                  & Have0( have3, b_G )
                  & copy_g_g( s_G, a_G, 0 )
                  & copy_g_h( b_G, a_G, 0 )
                  & copy_h_h( s_G, b_G, 0 )
                )
           )
    )

/*
                                    |--------------|
                                    | T2   ||  H3  |
                                    |      ||      |
                                    | a_G  || b_G  |
                                    |      ||      |
                                    |      ||      |
                                    |      ||      |
                                    |--------------|
                                          s_G   W23
*/

);

#size Have0;
#timer;


 // ####    ###   #   #  #   #  ####                   #
 // #   #  #   #  #   #  #   #   #  #                 ##
 // #   #  #   #  #   #  ##  #   #  #                # #
 // ####   #   #  #   #  # # #   #  #                  #
 // # #    #   #  #   #  #  ##   #  #                  #
 // #  #   #   #  #   #  #   #   #  #                  #
 // #   #   ###    ###   #   #  ####                 #####





// ###   #  #  ###   ####   ##   ###            #
//  #    #  #  #  #  #     #  #  #  #          ##
//  #    ####  #  #  ###   #  #  #  #           #
//  #    #  #  ###   #     ####  #  #           #
//  #    #  #  # #   #     #  #  #  #           #
//  #    #  #  #  #  ####  #  #  ###           ###


mu bool Thread1_1(
 blocktype  s_block,               // Block as appeared in Figure 2 of cav2010
 Module     s_mod,                 // Current module (thread)
 PrCount    s_pc,                  // Program counter
 Local      s_CL,                  // Local variable
 Globals    s_G                    // Global variable
)
 s_block <  s_mod,
 s_mod   <  s_pc,
 s_pc    <  s_CL,
 s_CL    <  s_G
( false

  // early termination
  | ( exists                   // There exists a state such that
            blocktype  t_block,
            Module     t_mod,
            PrCount    t_pc,
            Local      t_CL,
            Globals    t_G.
        (   Thread1_1( t_block, t_mod, t_pc, t_CL, t_G )    // That state in fixed point and ...
          & (   target( t_mod, t_pc )                // target is reached
              & ( false
                  |  t_block=thread1
                )
            )
        )
     )



  /*********************************************************************************/
  // thread1 (first thread) (TLI)


  // increase round_number for thread1
  | (    s_block=thread1  // This block is thread1 if
         & (
              (
                true
                & Thread1_0( thread1, s_mod, s_pc, s_CL, s_G ) // There exist thread1 at previous round
                & ( exists Globals a_G.
                       (
                            Have0( have23, a_G ) // There is RLI at previous round
                           & copy_g_h( a_G, s_G, 0 )    // And input of RLI is the same output TLI (thread1)
                           & folding( s_G, a_G, 1 )     // And output of RLI is wrapped at TLI (thread1)
                       )
                    )
              )
          )
         // Making sure that output and input match
         & s_G.h1 = s_G.g1
    )

/*
                                    |------||------|
                                    | T1   ||  H23 |
                                    |      ||      |
                                    | s_G  || a_G  |
                                    |      ||      |
                              t_r:  |      ||------|
                                    |      ||
                              s_r:  |------||
*/



//*********************************************************************************/
// forward propagation on internal transitions
//*********************************************************************************/


//*************** Round 1
  |  (
        (
          s_G.g1.v1=0
          & s_block=thread1
        )
      & (exists
           PrCount t_pc,
           Local   t_CL,
           Globals t_G.
           (    ( Thread1_1( s_block, s_mod, t_pc, t_CL, t_G )
                  & copy_g_and_h_1(s_G,t_G)
                )
               &(t_G.h1.v1=0 | ( t_G.h1.v1=1 & t_CL.v0=1 ) )
               &(
                  ( programInt1(s_mod,t_pc,s_pc,t_CL,s_CL,t_G.h1,s_G.h1)
                    & CopyVariables_ProgramInt(s_mod,t_pc,t_CL,s_CL,t_G.h1,s_G.h1)
                  )
                  | programInt3( s_mod, t_pc, s_pc, t_CL, s_CL, t_G.h1, s_G.h1 )
                )
           )
      )
    )
  | (
       ( s_G.g1.v1=0
        & s_block=thread1
       )
     & (s_G.h1.v1=0 | ( s_G.h1.v1=1 & s_CL.v0=1 ) )
     & (exists PrCount t_pc.
           (     Thread1_1( s_block, s_mod, t_pc, s_CL, s_G )
               & programInt2(s_mod,t_pc,s_pc,s_CL,s_G.h1)
           )
       )
    )
);
#size Thread1_1;
#timer;


bool ForgetLoc_r1_t1(     // Round 0 thread 1
 Globals    s_G                    // Global variable
)
(

   // forgetting local states for  each thread
  ( exists blocktype  t_block,
             Module     t_mod,
             PrCount    t_pc,
             Local      t_CL,
             Globals    t_G.
             (    Thread1_1( t_block, t_mod, t_pc, t_CL, t_G )
                & ( false
                    | (t_block=thread1)
                  )
                & copy_g_g( t_G, s_G, 1 )
                & copy_h_h( t_G, s_G, 1 )
                & (
                      t_G.h1.v1=0   // Atomic:=F
                  )
             )
    )
);

#size ForgetLoc_r1_t1;
#timer;

// ###   #  #  ###   ####   ##   ###                ##
//  #    #  #  #  #  #     #  #  #  #              #  #
//  #    ####  #  #  ###   #  #  #  #                 #
//  #    #  #  ###   #     ####  #  #                #
//  #    #  #  # #   #     #  #  #  #               #
//  #    #  #  #  #  ####  #  #  ###               ####

mu bool Thread2_1(
 blocktype  s_block,               // Block as appeared in Figure 2 of cav2010
 Module     s_mod,                 // Current module (thread)
 PrCount    s_pc,                  // Program counter
 Local      s_CL,                  // Local variable
 Globals    s_G                    // Global variable
)
 s_block <  s_mod,
 s_mod   <  s_pc,
 s_pc    <  s_CL,
 s_CL    <  s_G
( false

  // early termination

  | ( exists                   // There exists a state such that
            blocktype  t_block,
            CS         t_r,
            Module     t_mod,
            PrCount    t_pc,
            Local      t_CL,
            Globals    t_G.
        (   Thread2_1( t_block, t_mod, t_pc, t_CL, t_G )    // That state in fixed point and ...
          & (   target( t_mod, t_pc )                // target is reached
              & ( false
                  |  t_block=thread2
                )
            )
        )
     )

  /*********************************************************************************/
  // thread2 (first thread in want block)

  // increase round_number for thread2
  | (   s_block=thread2    // Throw to fixed point thread2 if
      & Thread2_0( thread2, s_mod, s_pc, s_CL, s_G )
      & ( exists Globals a_G,
                 Globals b_G.
            (
                ForgetLoc_r1_t1( b_G )
              & copy_g_h( s_G, b_G, 1 )
              & Have0( have3, a_G )
              & copy_g_h( a_G, s_G, 0 )
              )
         )
      & s_G.h1 = s_G.g1
    )

/*
                                     ------------------
                                    |  ------  ------  |
                                    | | T2   ||  H3  | |
                                    | |      ||      | |
                                    | | s_G  || a_G  | |
                                    | |      ||      | |
                                    |  ------  ------  |
                                    |        b_G    W23|
                                     -----------------
*/


//*************** Round 1
  |  (
        (
          s_G.g1.v1=0 & s_block=thread2
        )
      & (exists
           PrCount t_pc,
           Local   t_CL,
           Globals t_G.
           (    ( Thread2_1( s_block, s_mod, t_pc, t_CL, t_G )
                  & copy_g_and_h_1(s_G,t_G)
                )
               &(t_G.h1.v1=0 | ( t_G.h1.v1=1 & t_CL.v0=1 ) )
               &(
                  ( programInt1(s_mod,t_pc,s_pc,t_CL,s_CL,t_G.h1,s_G.h1)
                    & CopyVariables_ProgramInt(s_mod,t_pc,t_CL,s_CL,t_G.h1,s_G.h1)
                  )
                  | programInt3( s_mod, t_pc, s_pc, t_CL, s_CL, t_G.h1, s_G.h1 )
                )
           )
      )
    )


  | (
       (
        s_G.g1.v1=0 & s_block=thread2
       )
     & (s_G.h1.v1=0 | ( s_G.h1.v1=1 & s_CL.v0=1 ) )
     & (exists PrCount t_pc.
           (     Thread2_1( s_block, s_mod, t_pc, s_CL, s_G )
               & programInt2(s_mod,t_pc,s_pc,s_CL,s_G.h1)
           )
       )
    )

);

#size Thread2_1;
#timer;

bool ForgetLoc_r1_t2(     // Round 1 thread 2
 Globals    s_G                    // Global variable
)
(

   // forgetting local states for  each thread
  ( exists blocktype  t_block,
             Module     t_mod,
             PrCount    t_pc,
             Local      t_CL,
             Globals    t_G.
             (    Thread2_1( t_block, t_mod, t_pc, t_CL, t_G )
                & ( false
                    | (t_block=thread2)
                  )
                & copy_g_g( t_G, s_G, 1 )
                & copy_h_h( t_G, s_G, 1 )
                & (
                      t_G.h1.v1=0   // Atomic:=F
                  )
             )
    )
);

#size ForgetLoc_r1_t2;
#timer;


// ###   #  #  ###   ####   ##   ###               ####
//  #    #  #  #  #  #     #  #  #  #                 #
//  #    ####  #  #  ###   #  #  #  #               ##
//  #    #  #  ###   #     ####  #  #                 #
//  #    #  #  # #   #     #  #  #  #              #  #
//  #    #  #  #  #  ####  #  #  ###                ##

mu bool Thread3_1(
 blocktype  s_block,               // Block as appeared in Figure 2 of cav2010
 Module     s_mod,                 // Current module (thread)
 PrCount    s_pc,                  // Program counter
 Local      s_CL,                  // Local variable
 Globals    s_G                    // Global variable
)
 s_block <  s_mod,
 s_mod   <  s_pc,
 s_pc    <  s_CL,
 s_CL    <  s_G
( false

  // early termination

  | ( exists                   // There exists a state such that
            blocktype  t_block,
            Module     t_mod,
            PrCount    t_pc,
            Local      t_CL,
            Globals    t_G.
        (   Thread3_1( t_block, t_mod, t_pc, t_CL, t_G )    // That state in fixed point and ...
          & (   target( t_mod, t_pc )                // target is reached
              & ( false
                  |  t_block=thread3
                )
            )
        )
     )


  /*********************************************************************************/
  // thread3 (final thread in RLI)

  // increase round_number for thread3
  | (
        s_block=thread3
      & Thread3_0( thread3, s_mod, s_pc, s_CL, s_G )
      & ( exists Globals a_G.
               (
                  ForgetLoc_r1_t2(a_G ) //WANT s_mod, s_pc, s_CL are free
                  & copy_g_h( s_G, a_G, 1 )
               )
           )
      & s_G.h1 = s_G.g1
    )

/*
                                     ............
                                     ' |------| '
                                     ' |  T3  | '
                                     ' |      | '
                                     ' |  s_G | '
                              t_r:   ' |------| '
                                     '  a_G   W3'
                              s_r:   '..........'
*/

//*********************************************************************************/
// forward propagation on internal transitions
//*********************************************************************************/

//*************** Round 1
  |  (
        (
          s_G.g1.v1=0 & s_block=thread3
        )
      & (exists
           PrCount t_pc,
           Local   t_CL,
           Globals t_G.
           (    ( Thread3_1( s_block, s_mod, t_pc, t_CL, t_G )
                  & copy_g_and_h_1(s_G,t_G)
                )
               &(t_G.h1.v1=0 | ( t_G.h1.v1=1 & t_CL.v0=1 ) )
               &(
                  ( programInt1(s_mod,t_pc,s_pc,t_CL,s_CL,t_G.h1,s_G.h1)
                    & CopyVariables_ProgramInt(s_mod,t_pc,t_CL,s_CL,t_G.h1,s_G.h1)
                  )
                  | programInt3( s_mod, t_pc, s_pc, t_CL, s_CL, t_G.h1, s_G.h1 )
                )
           )
      )
    )


  | (
       (
        s_G.g1.v1=0 & s_block=thread3
        )
     & (s_G.h1.v1=0 | ( s_G.h1.v1=1 & s_CL.v0=1 ) )
     & (exists PrCount t_pc.
           (     Thread3_1( s_block, s_mod, t_pc, s_CL, s_G )
               & programInt2(s_mod,t_pc,s_pc,s_CL,s_G.h1)
           )
       )
    )

);

#size Thread3_1;
#timer;

bool ForgetLoc_r1_t3(     // Round 1 thread 2
 Globals    s_G                    // Global variable
)
(

   // forgetting local states for  each thread
  ( exists blocktype  t_block,
             Module     t_mod,
             PrCount    t_pc,
             Local      t_CL,
             Globals    t_G.
             (    Thread3_1( t_block, t_mod, t_pc, t_CL, t_G )
                & ( false
                    | (t_block=thread3)
                  )
                & copy_g_g( t_G, s_G, 1 )
                & copy_h_h( t_G, s_G, 1 )
                & (
                      t_G.h1.v1=0   // Atomic:=F
                  )
             )
    )
);

#size ForgetLoc_r1_t3;
#timer;


// #  #   ##   #  #  ####               #
// #  #  #  #  #  #  #                 ##
// ####  #  #  #  #  ###                #
// #  #  ####  #  #  #                  #
// #  #  #  #   ##   #                  #
// #  #  #  #   ##   ####              ###

mu bool Have1(    // Have block at round 1
 blocktype  s_block,
 Globals    s_G                    // Global variable
)
s_block < s_G
( false

  /*********************************************************************************/
  // have block (backward phase)

  // have blocks generated from the thread3 (stop forward propagation)
  | (    s_block=have3                  // This block will become have block (RLI if
         & ForgetLoc_r1_t3(s_G)
    )


/*
                                    |------|        |------|
                                    | T3   |        | T3   |
                                    |      |        |      |
                                    | s_G  | become | s_G  |
                                    |      |  -->   |      |
                                    |      |        |      |
                                    |      |        |      |
                                    |------|        |------|
                                        W3             H3
*/

  // have blocks generated by thread2
  | (    s_block=have23
         & ( exists Globals b_G,
                    Globals a_G.
                (
                    ForgetLoc_r1_t2(a_G)
                  & Have1( have3, b_G )
                  & copy_g_g( s_G, a_G, 1 )
                  & copy_g_h( b_G, a_G, 1 )
                  & copy_h_h( s_G, b_G, 1 )
                )
           )
    )

/*
                                    |--------------|
                                    | T2   ||  H3  |
                                    |      ||      |
                                    | a_G  || b_G  |
                                    |      ||      |
                                    |      ||      |
                                    |      ||      |
                                    |--------------|
                                          s_G   W23
*/

);

#size Have1;
#timer;




 // ####    ###   #   #  #   #  ####                  ###
 // #   #  #   #  #   #  #   #   #  #                #   #
 // #   #  #   #  #   #  ##  #   #  #                    #
 // ####   #   #  #   #  # # #   #  #                  ##
 // # #    #   #  #   #  #  ##   #  #                 #
 // #  #   #   #  #   #  #   #   #  #                #
 // #   #   ###    ###   #   #  ####                 #####


// ###   #  #  ###   ####   ##   ###                #
//  #    #  #  #  #  #     #  #  #  #              ##
//  #    ####  #  #  ###   #  #  #  #               #
//  #    #  #  ###   #     ####  #  #               #
//  #    #  #  # #   #     #  #  #  #               #
//  #    #  #  #  #  ####  #  #  ###               ###

mu bool Thread1_2(
 blocktype  s_block,               // Block as appeared in Figure 2 of cav2010
 Module     s_mod,                 // Current module (thread)
 PrCount    s_pc,                  // Program counter
 Local      s_CL,                  // Local variable
 Globals    s_G                    // Global variable
)
 s_block <  s_mod,
 s_mod   <  s_pc,
 s_pc    <  s_CL,
 s_CL    <  s_G
( false

  // early termination
  | ( exists                   // There exists a state such that
            blocktype  t_block,
            Module     t_mod,
            PrCount    t_pc,
            Local      t_CL,
            Globals    t_G.
        (   Thread1_2( t_block, t_mod, t_pc, t_CL, t_G )    // That state in fixed point and ...
          & (   target( t_mod, t_pc )                // target is reached
              & ( false
                  |  t_block=thread1
                )
            )
        )
     )



  /*********************************************************************************/
  // thread1 (first thread) (TLI)


  // increase round_number for thread1
  | (    s_block=thread1  // This block is thread1 if
         & (
              (
                true
                & Thread1_1( thread1, s_mod, s_pc, s_CL, s_G ) // There exist thread1 at previous round
                & ( exists Globals a_G.
                       (
                            Have1( have23, a_G ) // There is RLI at previous round
                           & copy_g_h( a_G, s_G, 1 )    // And input of RLI is the same output TLI (thread1)
                           & folding( s_G, a_G, 2 )     // And output of RLI is wrapped at TLI (thread1)
                       )
                    )
              )
          )
         // Making sure that output in input match
         & s_G.h2 = s_G.g2
    )

/*
                                    |------||------|
                                    | T1   ||  H23 |
                                    |      ||      |
                                    | s_G  || a_G  |
                                    |      ||      |
                              t_r:  |      ||------|
                                    |      ||
                              s_r:  |------||
*/



//*********************************************************************************/
// forward propagation on internal transitions
//*********************************************************************************/

//*************** Round 2
  |  (
        (
          s_G.g2.v1=0 & s_block=thread1
        )
      & (exists
           PrCount t_pc,
           Local   t_CL,
           Globals t_G.
           (    ( Thread1_2( s_block, s_mod, t_pc, t_CL, t_G )
                  & copy_g_and_h_2(s_G,t_G)
                )
               &(t_G.h2.v1=0 | ( t_G.h2.v1=1 & t_CL.v0=1 ) )
               &(
                  ( programInt1(s_mod,t_pc,s_pc,t_CL,s_CL,t_G.h2,s_G.h2)
                    & CopyVariables_ProgramInt(s_mod,t_pc,t_CL,s_CL,t_G.h2,s_G.h2)
                  )
                  | programInt3( s_mod, t_pc, s_pc, t_CL, s_CL, t_G.h2, s_G.h2 )
                )
           )
      )
    )

  | (
       ( s_G.g2.v1=0 & s_block=thread1
        )
     & (s_G.h2.v1=0 | ( s_G.h2.v1=1 & s_CL.v0=1 ) )
     & (exists PrCount t_pc.
           (     Thread1_2( s_block, s_mod, t_pc, s_CL, s_G )
               & programInt2(s_mod,t_pc,s_pc,s_CL,s_G.h2)
           )
       )
    )

);
#size Thread1_2;
#timer;


bool ForgetLoc_r2_t1(     // Round 0 thread 1
 Globals    s_G                    // Global variable
)
(

   // forgetting local states for  each thread
  ( exists blocktype  t_block,
             Module     t_mod,
             PrCount    t_pc,
             Local      t_CL,
             Globals    t_G.
             (    Thread1_2( t_block, t_mod, t_pc, t_CL, t_G )
                & ( false
                    | (t_block=thread1)
                  )
                & copy_g_g( t_G, s_G, 2 )
                & copy_h_h( t_G, s_G, 2 )
                & (
                      t_G.h2.v1=0   // Atomic:=F
                  )
             )
    )
);

#size ForgetLoc_r2_t1;
#timer;


// ###   #  #  ###   ####   ##   ###                ##
//  #    #  #  #  #  #     #  #  #  #              #  #
//  #    ####  #  #  ###   #  #  #  #                 #
//  #    #  #  ###   #     ####  #  #                #
//  #    #  #  # #   #     #  #  #  #               #
//  #    #  #  #  #  ####  #  #  ###               ####

mu bool Thread2_2(
 blocktype  s_block,               // Block as appeared in Figure 2 of cav2010
 Module     s_mod,                 // Current module (thread)
 PrCount    s_pc,                  // Program counter
 Local      s_CL,                  // Local variable
 Globals    s_G                    // Global variable
)
 s_block <  s_mod,
 s_mod   <  s_pc,
 s_pc    <  s_CL,
 s_CL    <  s_G
( false

  // early termination

  | ( exists                   // There exists a state such that
            blocktype  t_block,
            CS         t_r,
            Module     t_mod,
            PrCount    t_pc,
            Local      t_CL,
            Globals    t_G.
        (   Thread2_2( t_block, t_mod, t_pc, t_CL, t_G )    // That state in fixed point and ...
          & (   target( t_mod, t_pc )                // target is reached
              & ( false
                  |  t_block=thread2
                )
            )
        )
     )

  /*********************************************************************************/
  // thread2 (first thread in want block)

  // increase round_number for thread2
  | (   s_block=thread2    // Throw to fixed point thread2 if
      & Thread2_1( thread2, s_mod, s_pc, s_CL, s_G )
      & ( exists Globals a_G,
                 Globals b_G.
            (
                ForgetLoc_r2_t1( b_G )
              & copy_g_h( s_G, b_G, 2 )
              & Have1( have3, a_G )
              & copy_g_h( a_G, s_G, 1 )
              )
         )
      & s_G.h2 = s_G.g2
    )

/*
                                     ------------------
                                    |  ------  ------  |
                                    | | T2   ||  H3  | |
                                    | |      ||      | |
                                    | | s_G  || a_G  | |
                                    | |      ||      | |
                                    |  ------  ------  |
                                    |        b_G    W23|
                                     -----------------
*/


//*************** Round 2
  |  (
        (
          s_G.g2.v1=0 & s_block=thread2
          )
      & (exists
           PrCount t_pc,
           Local   t_CL,
           Globals t_G.
           (    ( Thread2_2( s_block, s_mod, t_pc, t_CL, t_G )
                  & copy_g_and_h_2(s_G,t_G)
                )
               &(t_G.h2.v1=0 | ( t_G.h2.v1=1 & t_CL.v0=1 ) )
               &(
                  ( programInt1(s_mod,t_pc,s_pc,t_CL,s_CL,t_G.h2,s_G.h2)
                    & CopyVariables_ProgramInt(s_mod,t_pc,t_CL,s_CL,t_G.h2,s_G.h2)
                  )
                  | programInt3( s_mod, t_pc, s_pc, t_CL, s_CL, t_G.h2, s_G.h2 )
                )
           )
      )
    )

  | (
       ( s_G.g2.v1=0 & s_block=thread2
        )
     & (s_G.h2.v1=0 | ( s_G.h2.v1=1 & s_CL.v0=1 ) )
     & (exists PrCount t_pc.
           (     Thread2_2( s_block, s_mod, t_pc, s_CL, s_G )
               & programInt2(s_mod,t_pc,s_pc,s_CL,s_G.h2)
           )
       )
    )

);

#size Thread2_2;
#timer;

bool ForgetLoc_r2_t2(     // Round 1 thread 2
 Globals    s_G                    // Global variable
)
(

   // forgetting local states for  each thread
  ( exists blocktype  t_block,
             Module     t_mod,
             PrCount    t_pc,
             Local      t_CL,
             Globals    t_G.
             (    Thread2_2( t_block, t_mod, t_pc, t_CL, t_G )
                & ( false
                    | (t_block=thread2)
                  )
                & copy_g_g( t_G, s_G, 2 )
                & copy_h_h( t_G, s_G, 2 )
                & (
                      t_G.h2.v1=0   // Atomic:=F
                  )
             )
    )
);

#size ForgetLoc_r2_t2;
#timer;

// ###   #  #  ###   ####   ##   ###               ####
//  #    #  #  #  #  #     #  #  #  #                 #
//  #    ####  #  #  ###   #  #  #  #               ##
//  #    #  #  ###   #     ####  #  #                 #
//  #    #  #  # #   #     #  #  #  #              #  #
//  #    #  #  #  #  ####  #  #  ###                ##

mu bool Thread3_2(
 blocktype  s_block,               // Block as appeared in Figure 2 of cav2010
 Module     s_mod,                 // Current module (thread)
 PrCount    s_pc,                  // Program counter
 Local      s_CL,                  // Local variable
 Globals    s_G                    // Global variable
)
 s_block <  s_mod,
 s_mod   <  s_pc,
 s_pc    <  s_CL,
 s_CL    <  s_G
( false

  // early termination

  | ( exists                   // There exists a state such that
            blocktype  t_block,
            Module     t_mod,
            PrCount    t_pc,
            Local      t_CL,
            Globals    t_G.
        (   Thread3_2( t_block, t_mod, t_pc, t_CL, t_G )    // That state in fixed point and ...
          & (   target( t_mod, t_pc )                // target is reached
              & ( false
                  |  t_block=thread3
                )
            )
        )
     )


  /*********************************************************************************/
  // thread3 (final thread in RLI)

  // increase round_number for thread3
  | (
        s_block=thread3
      & Thread3_1( thread3, s_mod, s_pc, s_CL, s_G )
      & ( exists Globals a_G.
               (
                  ForgetLoc_r2_t2(a_G ) //WANT s_mod, s_pc, s_CL are free
                  & copy_g_h( s_G, a_G, 2 )
               )
           )
      & s_G.h2 = s_G.g2
    )

/*
                                     ............
                                     ' |------| '
                                     ' |  T3  | '
                                     ' |      | '
                                     ' |  s_G | '
                              t_r:   ' |------| '
                                     '  a_G   W3'
                              s_r:   '..........'
*/

//*********************************************************************************/
// forward propagation on internal transitions
//*********************************************************************************/

//*************** Round 2
  |  (
        (
          s_G.g2.v1=0 & s_block=thread3
           )
      & (exists
           PrCount t_pc,
           Local   t_CL,
           Globals t_G.
           (    ( Thread3_2( s_block, s_mod, t_pc, t_CL, t_G )
                  & copy_g_and_h_2(s_G,t_G)
                )
               &(t_G.h2.v1=0 | ( t_G.h2.v1=1 & t_CL.v0=1 ) )
               &(
                  ( programInt1(s_mod,t_pc,s_pc,t_CL,s_CL,t_G.h2,s_G.h2)
                    & CopyVariables_ProgramInt(s_mod,t_pc,t_CL,s_CL,t_G.h2,s_G.h2)
                  )
                  | programInt3( s_mod, t_pc, s_pc, t_CL, s_CL, t_G.h2, s_G.h2 )
                )
           )
      )
    )


  | (
       ( s_G.g2.v1=0 & s_block=thread3
        )
     & (s_G.h2.v1=0 | ( s_G.h2.v1=1 & s_CL.v0=1 ) )
     & (exists PrCount t_pc.
           (     Thread3_2( s_block, s_mod, t_pc, s_CL, s_G )
               & programInt2(s_mod,t_pc,s_pc,s_CL,s_G.h2)
           )
       )
    )

);

#size Thread3_2;
#timer;

bool ForgetLoc_r2_t3(     // Round 2 thread 3
 Globals    s_G                    // Global variable
)
(

   // forgetting local states for  each thread
  ( exists blocktype  t_block,
             Module     t_mod,
             PrCount    t_pc,
             Local      t_CL,
             Globals    t_G.
             (    Thread3_2( t_block, t_mod, t_pc, t_CL, t_G )
                & ( false
                    | (t_block=thread3)
                  )
                & copy_g_g( t_G, s_G, 2 )
                & copy_h_h( t_G, s_G, 2 )
                & (
                      t_G.h2.v1=0   // Atomic:=F
                  )
             )
    )
);

#size ForgetLoc_r2_t3;
#timer;

// #  #   ##   #  #  ####               ##
// #  #  #  #  #  #  #                 #  #
// ####  #  #  #  #  ###                  #
// #  #  ####  #  #  #                   #
// #  #  #  #   ##   #                  #
// #  #  #  #   ##   ####              ####
mu bool Have2(    // Have block at round 1
 blocktype  s_block,
 Globals    s_G                    // Global variable
)
s_block < s_G
( false

  /*********************************************************************************/
  // have block (backward phase)

  // have blocks generated from the thread3 (stop forward propagation)
  | (    s_block=have3                  // This block will become have block (RLI if
         & ForgetLoc_r2_t3(s_G)
    )


/*
                                    |------|        |------|
                                    | T3   |        | T3   |
                                    |      |        |      |
                                    | s_G  | become | s_G  |
                                    |      |  -->   |      |
                                    |      |        |      |
                                    |      |        |      |
                                    |------|        |------|
                                        W3             H3
*/

  // have blocks generated by thread2
  | (    s_block=have23
         & ( exists Globals b_G,
                    Globals a_G.
                (
                    ForgetLoc_r2_t2(a_G)
                  & Have2( have3, b_G )
                  & copy_g_g( s_G, a_G, 2 )
                  & copy_g_h( b_G, a_G, 2 )
                  & copy_h_h( s_G, b_G, 2 )
                )
           )
    )

/*
                                    |--------------|
                                    | T2   ||  H3  |
                                    |      ||      |
                                    | a_G  || b_G  |
                                    |      ||      |
                                    |      ||      |
                                    |      ||      |
                                    |--------------|
                                          s_G   W23
*/

);

#size Have2;
#timer;


 // ####    ###   #   #  #   #  ####          #####
 // #   #  #   #  #   #  #   #   #  #             #
 // #   #  #   #  #   #  ##  #   #  #            #
 // ####   #   #  #   #  # # #   #  #           ##
 // # #    #   #  #   #  #  ##   #  #             #
 // #  #   #   #  #   #  #   #   #  #         #   #
 // #   #   ###    ###   #   #  ####           ###

// ###   #  #  ###   ####   ##   ###                #
//  #    #  #  #  #  #     #  #  #  #              ##
//  #    ####  #  #  ###   #  #  #  #               #
//  #    #  #  ###   #     ####  #  #               #
//  #    #  #  # #   #     #  #  #  #               #
//  #    #  #  #  #  ####  #  #  ###               ###

mu bool Thread1_3(
 blocktype  s_block,               // Block as appeared in Figure 2 of cav2010
 Module     s_mod,                 // Current module (thread)
 PrCount    s_pc,                  // Program counter
 Local      s_CL,                  // Local variable
 Globals    s_G                    // Global variable
)
 s_block <  s_mod,
 s_mod   <  s_pc,
 s_pc    <  s_CL,
 s_CL    <  s_G
( false

  // early termination
  | ( exists                   // There exists a state such that
            blocktype  t_block,
            Module     t_mod,
            PrCount    t_pc,
            Local      t_CL,
            Globals    t_G.
        (   Thread1_3( t_block, t_mod, t_pc, t_CL, t_G )    // That state in fixed point and ...
          & (   target( t_mod, t_pc )                // target is reached
              & ( false
                  |  t_block=thread1
                )
            )
        )
     )



  /*********************************************************************************/
  // thread1 (first thread) (TLI)


  // increase round_number for thread1
  | (    s_block=thread1  // This block is thread1 if
         & (
              (
                true
                & Thread1_2( thread1, s_mod, s_pc, s_CL, s_G ) // There exist thread1 at previous round
                & ( exists Globals a_G.
                       (
                            Have2( have23, a_G ) // There is RLI at previous round
                           & copy_g_h( a_G, s_G, 2 )    // And input of RLI is the same output TLI (thread1)
                           & folding( s_G, a_G, 3 )     // And output of RLI is wrapped at TLI (thread1)
                       )
                    )
              )
          )
         // Making sure that output in input match
         & s_G.h3 = s_G.g3
    )

/*
                                    |------||------|
                                    | T1   ||  H23 |
                                    |      ||      |
                                    | s_G  || a_G  |
                                    |      ||      |
                              t_r:  |      ||------|
                                    |      ||
                              s_r:  |------||
*/



//*********************************************************************************/
// forward propagation on internal transitions
//*********************************************************************************/

//*************** Round 3

  |  (  (s_G.g3.v1=0 & s_block=thread1)
      & (exists
           PrCount t_pc,
           Local   t_CL,
           Globals t_G.
           (    (   Thread1_3( s_block, s_mod, t_pc, t_CL, t_G )
                  & copy_g_and_h_3(s_G,t_G)
                )
               &(t_G.h3.v1=0 | ( t_G.h3.v1=1 & t_CL.v0=1 ) )
               &(
                  ( programInt1(s_mod,t_pc,s_pc,t_CL,s_CL,t_G.h3,s_G.h3)
                    & CopyVariables_ProgramInt(s_mod,t_pc,t_CL,s_CL,t_G.h3,s_G.h3)
                  )
                  | programInt3( s_mod, t_pc, s_pc, t_CL, s_CL, t_G.h3, s_G.h3 )
                )
           )
      )
    )

  | (  (s_G.g3.v1=0 & s_block=thread1)
     & (s_G.h3.v1=0 | ( s_G.h3.v1=1 & s_CL.v0=1 ) )
     & (exists PrCount t_pc.
           (     Thread1_3( s_block, s_mod, t_pc, s_CL, s_G )
               & programInt2(s_mod,t_pc,s_pc,s_CL,s_G.h3)
           )
       )
    )
);
#size Thread1_3;
#timer;


bool ForgetLoc_r3_t1(     // Round 0 thread 1
 Globals    s_G                    // Global variable
)
(

   // forgetting local states for  each thread
  ( exists blocktype  t_block,
             Module     t_mod,
             PrCount    t_pc,
             Local      t_CL,
             Globals    t_G.
             (    Thread1_3( t_block, t_mod, t_pc, t_CL, t_G )
                & ( false
                    | (t_block=thread1)
                  )
                & copy_g_g( t_G, s_G, 3 )
                & copy_h_h( t_G, s_G, 3 )
                & (
                      t_G.h3.v1=0   // Atomic:=F
                  )
             )
    )
);

#size ForgetLoc_r3_t1;
#timer;


// ###   #  #  ###   ####   ##   ###                ##
//  #    #  #  #  #  #     #  #  #  #              #  #
//  #    ####  #  #  ###   #  #  #  #                 #
//  #    #  #  ###   #     ####  #  #                #
//  #    #  #  # #   #     #  #  #  #               #
//  #    #  #  #  #  ####  #  #  ###               ####

mu bool Thread2_3(
 blocktype  s_block,               // Block as appeared in Figure 2 of cav2010
 Module     s_mod,                 // Current module (thread)
 PrCount    s_pc,                  // Program counter
 Local      s_CL,                  // Local variable
 Globals    s_G                    // Global variable
)
 s_block <  s_mod,
 s_mod   <  s_pc,
 s_pc    <  s_CL,
 s_CL    <  s_G
( false

  // early termination

  | ( exists                   // There exists a state such that
            blocktype  t_block,
            CS         t_r,
            Module     t_mod,
            PrCount    t_pc,
            Local      t_CL,
            Globals    t_G.
        (
          Thread2_3( t_block, t_mod, t_pc, t_CL, t_G )
          & (   target( t_mod, t_pc )
              & ( false
                  |  t_block=thread2
                )
            )
        )
     )

  // thread2 (first thread in want block)

  // increase round_number for thread2
  | (   s_block=thread2    // Throw to fixed point thread2 if
      & Thread2_2( thread2, s_mod, s_pc, s_CL, s_G )
      & ( exists Globals a_G,
                 Globals b_G.
            (
                ForgetLoc_r3_t1( b_G )
              & copy_g_h( s_G, b_G, 3 )
              & Have2( have3, a_G )
              & copy_g_h( a_G, s_G, 2 )
              )
         )
      & s_G.h3 = s_G.g3
    )

/*
                                     ------------------
                                    |  ------  ------  |
                                    | | T2   ||  H3  | |
                                    | |      ||      | |
                                    | | s_G  || a_G  | |
                                    | |      ||      | |
                                    |  ------  ------  |
                                    |        b_G    W23|
                                     -----------------
*/


// Round 3

  |  (  (s_G.g3.v1=0 & s_block=thread2 )
      & (exists
           PrCount t_pc,
           Local   t_CL,
           Globals t_G.
           (    (   Thread2_3( s_block, s_mod, t_pc, t_CL, t_G )
                  & copy_g_and_h_3(s_G,t_G)
                )
               &(t_G.h3.v1=0 | ( t_G.h3.v1=1 & t_CL.v0=1 ) )
               &(
                  ( programInt1(s_mod,t_pc,s_pc,t_CL,s_CL,t_G.h3,s_G.h3)
                    & CopyVariables_ProgramInt(s_mod,t_pc,t_CL,s_CL,t_G.h3,s_G.h3)
                  )
                  | programInt3( s_mod, t_pc, s_pc, t_CL, s_CL, t_G.h3, s_G.h3 )
                )
           )
      )
    )

  | (  (s_G.g3.v1=0 & s_block=thread2)
     & (s_G.h3.v1=0 | ( s_G.h3.v1=1 & s_CL.v0=1 ) )
     & (exists PrCount t_pc.
           (     Thread2_3( s_block, s_mod, t_pc, s_CL, s_G )
               & programInt2(s_mod,t_pc,s_pc,s_CL,s_G.h3)
           )
       )
    )

);
#size Thread2_3;
#timer;


bool ForgetLoc_r3_t2(     // Round 1 thread 2
 Globals    s_G                    // Global variable
)
(

   // forgetting local states for  each thread
  ( exists blocktype  t_block,
             Module     t_mod,
             PrCount    t_pc,
             Local      t_CL,
             Globals    t_G.
             (    Thread2_3( t_block, t_mod, t_pc, t_CL, t_G )
                & ( false
                    | (t_block=thread2)
                  )
                & copy_g_g( t_G, s_G, 3 )
                & copy_h_h( t_G, s_G, 3 )
                & (
                      t_G.h3.v1=0   // Atomic:=F
                  )
             )
    )
);

#size ForgetLoc_r3_t2;
#timer;

// ###   #  #  ###   ####   ##   ###         ####
//  #    #  #  #  #  #     #  #  #  #           #
//  #    ####  #  #  ###   #  #  #  #         ##
//  #    #  #  ###   #     ####  #  #           #
//  #    #  #  # #   #     #  #  #  #        #  #
//  #    #  #  #  #  ####  #  #  ###          ##

mu bool Thread3_3(
 blocktype  s_block,               // Block as appeared in Figure 2 of cav2010
 Module     s_mod,                 // Current module (thread)
 PrCount    s_pc,                  // Program counter
 Local      s_CL,                  // Local variable
 Globals    s_G                    // Global variable
)
 s_block <  s_mod,
 s_mod   <  s_pc,
 s_pc    <  s_CL,
 s_CL    <  s_G
( false

  // early termination

  | ( exists                   // There exists a state such that
            blocktype  t_block,
            Module     t_mod,
            PrCount    t_pc,
            Local      t_CL,
            Globals    t_G.
        (   Thread3_3( t_block, t_mod, t_pc, t_CL, t_G )    // That state in fixed point and ...
          & (   target( t_mod, t_pc )                // target is reached
              & ( false
                  |  t_block=thread3
                )
            )
        )
     )


  /*********************************************************************************/
  // thread3 (final thread in RLI)

  // increase round_number for thread3
  | (
        s_block=thread3
      & Thread3_2( thread3, s_mod, s_pc, s_CL, s_G )
      & ( exists Globals a_G.
               (
                  ForgetLoc_r3_t2(a_G ) //WANT s_mod, s_pc, s_CL are free
                  & copy_g_h( s_G, a_G, 3 )
               )
           )
      & s_G.h3 = s_G.g3
    )

/*
                                     ............
                                     ' |------| '
                                     ' |  T3  | '
                                     ' |      | '
                                     ' |  s_G | '
                              t_r:   ' |------| '
                                     '  a_G   W3'
                              s_r:   '..........'
*/

//*********************************************************************************/
// forward propagation on internal transitions
//*********************************************************************************/
//*************** Round 3

  |  (  (s_G.g3.v1=0 & s_block=thread3)
      & (exists
           PrCount t_pc,
           Local   t_CL,
           Globals t_G.
           (    (   Thread3_3( s_block, s_mod, t_pc, t_CL, t_G )
                  & copy_g_and_h_3(s_G,t_G)
                )
               &(t_G.h3.v1=0 | ( t_G.h3.v1=1 & t_CL.v0=1 ) )
               &(
                  ( programInt1(s_mod,t_pc,s_pc,t_CL,s_CL,t_G.h3,s_G.h3)
                    & CopyVariables_ProgramInt(s_mod,t_pc,t_CL,s_CL,t_G.h3,s_G.h3)
                  )
                  | programInt3( s_mod, t_pc, s_pc, t_CL, s_CL, t_G.h3, s_G.h3 )
                )
           )
      )
    )

  | (  (s_G.g3.v1=0 & s_block=thread3)
     & (s_G.h3.v1=0 | ( s_G.h3.v1=1 & s_CL.v0=1 ) )
     & (exists PrCount t_pc.
           (     Thread3_3( s_block, s_mod, t_pc, s_CL, s_G )
               & programInt2(s_mod,t_pc,s_pc,s_CL,s_G.h3)
           )
       )
    )
);

#size Thread3_3;
#timer;


bool ForgetLoc_r3_t3(     // Round 2 thread 3
 Globals    s_G                    // Global variable
)
(

   // forgetting local states for  each thread
  ( exists blocktype  t_block,
             Module     t_mod,
             PrCount    t_pc,
             Local      t_CL,
             Globals    t_G.
             (    Thread3_3( t_block, t_mod, t_pc, t_CL, t_G )
                & ( false
                    | (t_block=thread3)
                  )
                & copy_g_g( t_G, s_G, 3 )
                & copy_h_h( t_G, s_G, 3 )
                & (
                      t_G.h3.v1=0   // Atomic:=F
                  )
             )
    )
);

#size ForgetLoc_r3_t3;
#timer;


// #  #   ##   #  #  ####        ####
// #  #  #  #  #  #  #              #
// ####  #  #  #  #  ###          ##
// #  #  ####  #  #  #              #
// #  #  #  #   ##   #           #  #
// #  #  #  #   ##   ####         ##
mu bool Have3(    // Have block at round 1
 blocktype  s_block,
 Globals    s_G                    // Global variable
)
s_block < s_G
( false

  /*********************************************************************************/
  // have block (backward phase)

  // have blocks generated from the thread3 (stop forward propagation)
  | (    s_block=have3                  // This block will become have block (RLI if
         & ForgetLoc_r3_t3(s_G)
    )


/*
                                    |------|        |------|
                                    | T3   |        | T3   |
                                    |      |        |      |
                                    | s_G  | become | s_G  |
                                    |      |  -->   |      |
                                    |      |        |      |
                                    |      |        |      |
                                    |------|        |------|
                                        W3             H3
*/

  // have blocks generated by thread2
  | (    s_block=have23
         & ( exists Globals b_G,
                    Globals a_G.
                (
                    ForgetLoc_r3_t2(a_G)
                  & Have3( have3, b_G )
                  & copy_g_g( s_G, a_G, 3 )
                  & copy_g_h( b_G, a_G, 3 )
                  & copy_h_h( s_G, b_G, 3 )
                )
           )
    )

/*
                                    |--------------|
                                    | T2   ||  H3  |
                                    |      ||      |
                                    | a_G  || b_G  |
                                    |      ||      |
                                    |      ||      |
                                    |      ||      |
                                    |--------------|
                                          s_G   W23
*/

);

#size Have3;
#timer;



/******************************************************************************************************/
//                                       Reachabibility check                                         *
/******************************************************************************************************/


(
    ( exists
            PrCount t_pc,
            Local   t_CL,
            Global  t_G.
        (   Init_Reach( t_pc, t_CL, t_G )
          & target( 0, t_pc)   // ??????????
        )
     )

  | ( exists
            blocktype  t_block,
            Module     t_mod,
            PrCount    t_pc,
            Local      t_CL,
            Globals    t_G.
        (
            (
                Thread1_3( t_block, t_mod, t_pc, t_CL, t_G )
              & target( 1, t_pc )
            )
            | (
                Thread2_3( t_block, t_mod, t_pc, t_CL, t_G )
              & target( 2, t_pc )
            )
            | (
                Thread3_3( t_block, t_mod, t_pc, t_CL, t_G )
              & target( 3, t_pc )
            )
        )
     )

);

#timer stop;

