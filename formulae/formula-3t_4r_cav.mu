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
                want23,      // Want block of t2 and t3
                want3,      // Want block
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


// module init is assigned s_mod=0
// Also change them in the translation
// These functions will change:
// Target_Init

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

bool GlobalInit(Global CG)           // Call Init on Global (shared) variable
( exists
         Module  s_mod,
         PrCount s_pc,
         Local   s_CL.
              (   Init_Reach( s_pc, s_CL, CG )
                & Target_Init(s_pc )
              )
);


/*
    Fixed point
  s_ : means current state
  t_ : means previous state
 */

mu bool Sequ_Reach(
 blocktype  s_block,               // Block as appeared in Figure 2 of cav2010
 CS         s_r,                   // Current round
 Module     s_mod,                 // Current module (thread)
 PrCount    s_pc,                  // Program counter
 Local      s_CL,                  // Local variable
 Globals    s_G                    // Global variable
)
 s_r     <  s_block,
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
        (   Sequ_Reach( t_block, t_r, t_mod, t_pc, t_CL, t_G )    // That state in fixed point and ...
          & (   target( t_mod, t_pc )                // target is reached
              & ( false
                  |  t_block=thread1
                  |  t_block=thread2
                  |  t_block=thread3
                )
            )
        )
     )


  /*********************************************************************************/

   // forgetting local states for  each thread
  | ( exists blocktype  t_block,
             Module     t_mod,
             PrCount    t_pc,
             Local      t_CL,
             Globals    t_G.
             (    Sequ_Reach( t_block, s_r, t_mod, t_pc, t_CL, t_G )
                & ( false
                    |  ( (s_block=thread1noloc) & (t_block=thread1) )
                    |  ( (s_block=thread2noloc) & (t_block=thread2) )
                    |  ( (s_block=thread3noloc) & (t_block=thread3) )
                  )
                & copy_g_g( t_G, s_G, s_r )
                & copy_h_h( t_G, s_G, s_r )
                & (
                      t_G.h0.v1=0
                    & t_G.h1.v1=0
                    & t_G.h2.v1=0
                    & t_G.h3.v1=0
                  )
             )
    )


/*
                                    |-----------------|
                                    |       T         |
                                    |                 |
                                    | s_G             |
                                    |                 |
                                    |                 |
                                    | g  ~~~------>  h|
                                    |-----------------|
*/


  /*********************************************************************************/
  // thread1 (first thread) (TLI)


  // initial configuration (init)
  |  (
         s_block = thread1              // Start thread1 if
         & initPC(s_pc)                 // Initial PC
         & s_mod=1                      // In module 1(thread1)
         & GlobalInit(s_G.g0)           // INIT runs FIRST
         & s_r=0                        // Round 0
         // At this state, input = output
         & s_G.h0=s_G.g0
         & s_G.h1=s_G.g1
         & s_G.h2=s_G.g2
         & s_G.h3=s_G.g3
         // & ( forall PrCount t_pc. (!target( 0, t_pc)))  // ??????????????/**/
     )


  // increase round_number for thread1
  | (    s_block=thread1  // This block is thread1 if
         & ( exists CS t_r.     // There exist a round t_r
              (   true
                  & increaseCS( t_r, s_r )   // That round is the previous round
                  & Sequ_Reach( thread1, t_r, s_mod, s_pc, s_CL, s_G ) // There exist thread1 at previous round
                  & ( exists Globals a_G.
                       (     Sequ_Reach( have23, t_r, s_mod, s_pc, s_CL, a_G ) // There is RLI at previous round
                           & copy_g_h( a_G, s_G, t_r )    // And input of RLI is the same output TLI (thread1)
                           & folding( s_G, a_G, s_r )     // And output of RLI is wrapped at TLI (thread1)
                       )
                    )
              )
          )
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

  /*********************************************************************************/
  // thread2 (first thread in want block)

  // initial configuration
  |  (   s_block=thread2     // This is thread2 if
         & s_r=0             // At the first round
         & initPC(s_pc)      // begin with initial pc
         & s_mod=2           // in module 2 (thread2)
         & ( exists  Globals t_G.
              (
                Sequ_Reach( thread1noloc, s_r, s_mod, s_pc, s_CL, t_G )    // After thread1 finishes its execution
                & t_G.h0 = s_G.g0    // Map output of thread1 to input of thread2
              )
           )
         // At this state, input = output
         & s_G.h0=s_G.g0
         & s_G.h1=s_G.g1
         & s_G.h2=s_G.g2
         & s_G.h3=s_G.g3
     )


  // increase round_number for thread2
  | (    s_block=thread2    // Throw to fixed point thread2 if
         & ( s_r!=0 )       // At some round > 0
         & ( exists CS t_r.   // There exists thread2 at previous round
               (
                    Sequ_Reach( thread2, t_r, s_mod, s_pc, s_CL, s_G )
                  & increaseCS( t_r, s_r )
               )
           )
         & ( exists Globals b_G.         // and there exists a WANT23 block with global b_G such that
              (   ( exists CS t_r, Globals a_G.
                      (   Sequ_Reach( have3, t_r, s_mod, s_pc, s_CL, a_G )  // There is RLI at previous round with
                        & (   copy_g_h( a_G, s_G, t_r )      // Output of thread2 is the same input of RLI
                            & copy_h_h( a_G, b_G, t_r )      // Output of RLI is the same as output of WANT block
                            & increaseCS( t_r, s_r )         // previous round
                          )
                      )
                  )
                & Sequ_Reach( want23, s_r, s_mod, s_pc, s_CL, b_G )  // thread2 is in WANT block
                & copy_g_g( b_G, s_G, s_r )                   // Input of thread2 is the same as input of WANT block
              )
           )
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


  /*********************************************************************************/
  // thread3 (final thread in RLI)

  // initial configuration (same as thread3)
  |  (   s_block=thread3
         & s_r=0
         & initPC(s_pc)
         & s_mod=3
         & ( exists  Globals t_G.
                  (
                    Sequ_Reach( thread2noloc, s_r, s_mod, s_pc, s_CL, t_G )
                    & t_G.h0 = s_G.g0
                  )
           )
         & s_G.h0=s_G.g0
         & s_G.h1=s_G.g1
         & s_G.h2=s_G.g2
         & s_G.h3=s_G.g3
     )


  // increase round_number for thread3
  | (    s_block=thread3
         & ( s_r!=0 )
         & ( exists CS t_r,
                    Globals a_G.
               (
                    Sequ_Reach( thread3, t_r, s_mod, s_pc, s_CL, s_G )
                  & increaseCS( t_r, s_r )

                  & Sequ_Reach( want3, s_r, s_mod, s_pc, s_CL, a_G ) //WANT s_mod, s_pc, s_CL are free
                  & copy_h_h( a_G, s_G, t_r )
                  & copy_g_g( a_G, s_G, s_r )
               )
           )
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


  /*********************************************************************************/
  // want block (forward phase)


  // want blocks generated by thread1

  | (    s_block=want23     // Thread1 asks for WANT block of thread2 and thread3
         & ( s_r!=0 )    //   round > 0
         & ( exists CS t_r.        // And this is already a HAVE block in previous round
              (   Sequ_Reach( have23, t_r, s_mod, s_pc, s_CL, s_G ) //HAVE
                & increaseCS( t_r, s_r )
              )
           )
         & ( exists Globals a_G.  // And there exists thread1 in which
              (   Sequ_Reach( thread1noloc, s_r, s_mod, s_pc, s_CL, a_G ) //T1 completes simulation in round s_r
                & (    copy_g_h( s_G, a_G, s_r )  // Thread1 output matches this block input
                     & folding( a_G, s_G, s_r )   // And this block output wrap thread1 input
                  )
              )
           )
    )

/*
                                                    ..............
                                      |------|      '  |------|  '
                                      | T1   |      '  |  H23 |  '
                                      |      |      '  |      |  '
                                      | a_G  |      '  | s_G  |  '
                                      |      |      '  |      |  '
                                t_r:  |      |      '  |------|  '
                                      |      |      '            '
                                s_r:  |      | ---> '        W23 '
                                      |------|      '............'

*/
  // want blocks generated by thread2

  | (    s_block=want3        // thread2 asks for WANT block of thread3
         & ( s_r!=0 )        // round > 0
         & ( exists CS t_r.  // and this block is a HAVE (RLI) in previous round
              (   Sequ_Reach( have3, t_r, s_mod, s_pc, s_CL, s_G ) //HAVE
                & increaseCS( t_r, s_r )
              )
           )
         & ( exists Globals b_G.   // Global of this block
              (   (exists Globals a_G.   // And there exist thread2 (end of internal transition)
                      (   Sequ_Reach( thread2noloc, s_r, s_mod, s_pc, s_CL, a_G ) //Thread2
                        & (    copy_g_h( s_G, a_G, s_r )  // Out put of thread 2 matches input of this block
                             & copy_g_g( a_G, b_G, s_r )  // Input of this block  is the input of thread2
                          )
                      )
                  )
                & Sequ_Reach( want23, s_r, s_mod, s_pc, s_CL, b_G )       // There is want block outside
                & copy_h_h( b_G, s_G, s_r )
              )
           )
    )

/*
                                   ..........................
                                   '             '          '
                                   ' |------|    ' |------| '
                                   ' | T2   |    ' |  H3  | '
                                   ' |      |    ' |      | '
                                   ' | a_G  |    ' | s_G  | '
                                   ' |      |    ' |      | '
                            t_r:   ' |      |    ' |------| '
                                   ' |------|--> '       W3 '
                            s_r:   '.............'..........'
                                           b_G         W23
*/

  /*********************************************************************************/
  // have block (backward phase)

  // have blocks generated from the thread3 (stop forward propagation)
  | (    s_block=have3                  // This block will become have block (RLI if
         & (
                (
                  Sequ_Reach( thread3noloc, s_r, s_mod, s_pc, s_CL, s_G )
                )
          )
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
         & (    s_r=0
              | Sequ_Reach( want23, s_r, s_mod, s_pc, s_CL, s_G )
           )
         & ( exists Globals b_G.
                  (     ( exists Globals a_G.
                            (    Sequ_Reach( thread2noloc, s_r, s_mod, s_pc, s_CL, a_G )
                               & (   copy_g_g( s_G, a_G, s_r )
                                   & copy_g_h( b_G, a_G, s_r )
                                 )
                            )
                        )
                     &  (    Sequ_Reach( have3, s_r, s_mod, s_pc, s_CL, b_G )
                           & copy_h_h( s_G, b_G, s_r )
                        )
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


//*********************************************************************************/
// forward propagation on internal transitions
//*********************************************************************************/


//*************** Round 0
  |  (
        (   s_r=0      // Round 0
          & s_G.g0.v1 =0     // Why throw in                                atomic=F?
          & (s_block=thread1 | s_block=thread2 | s_block=thread3)   // Throw in any thread
        )
      & (exists                  // There exists an internal state that
           PrCount t_pc,
           Local   t_CL,
           Globals t_G.
           (    (   Sequ_Reach( s_block, s_r, s_mod, t_pc, t_CL, t_G )  // s_r and s_mod remain
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
          s_r=0
          & s_G.g0.v1 =0
          & (s_block=thread1 | s_block=thread2 | s_block=thread3)
        )
       & (s_G.h0.v1=0 | ( s_G.h0.v1=1 & s_CL.v0=1 ) )     // Atomic condition
       & (exists PrCount t_pc.
             (     Sequ_Reach( s_block, s_r, s_mod, t_pc, s_CL, s_G )
                 & programInt2(s_mod,t_pc,s_pc,s_CL,s_G.h0)    // Control flow statement
             )
         )
    )



//*************** Round 1
  |  (
        (s_r=1  &  s_G.g1.v1=0 & (s_block=thread1 | s_block=thread2 | s_block=thread3) )
      & (exists
           PrCount t_pc,
           Local   t_CL,
           Globals t_G.
           (    ( Sequ_Reach( s_block, s_r, s_mod, t_pc, t_CL, t_G )
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
       ( s_G.g1.v1=0 & s_r=1 & (s_block=thread1 | s_block=thread2 | s_block=thread3) )
     & (s_G.h1.v1=0 | ( s_G.h1.v1=1 & s_CL.v0=1 ) )
     & (exists PrCount t_pc.
           (     Sequ_Reach( s_block, s_r, s_mod, t_pc, s_CL, s_G )
               & programInt2(s_mod,t_pc,s_pc,s_CL,s_G.h1)
           )
       )
    )




//*************** Round 2
  |  (
        ( s_r=2  &  s_G.g2.v1=0 & (s_block=thread1 | s_block=thread2 | s_block=thread3) )
      & (exists
           PrCount t_pc,
           Local   t_CL,
           Globals t_G.
           (    ( Sequ_Reach( s_block, s_r, s_mod, t_pc, t_CL, t_G )
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
       ( s_G.g2.v1=0 & s_r=2 & (s_block=thread1 | s_block=thread2 | s_block=thread3) )
     & (s_G.h2.v1=0 | ( s_G.h2.v1=1 & s_CL.v0=1 ) )
     & (exists PrCount t_pc.
           (     Sequ_Reach( s_block, s_r, s_mod, t_pc, s_CL, s_G )
               & programInt2(s_mod,t_pc,s_pc,s_CL,s_G.h2)
           )
       )
    )




//*************** Round 3

  |  (  (s_r=3  &  s_G.g3.v1=0 & (s_block=thread1 | s_block=thread2 | s_block=thread3) )
      & (exists
           PrCount t_pc,
           Local   t_CL,
           Globals t_G.
           (    (   Sequ_Reach( s_block, s_r, s_mod, t_pc, t_CL, t_G )
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

  | (  (s_G.g3.v1=0 & s_r=3 & (s_block=thread1 | s_block=thread2 | s_block=thread3) )
     & (s_G.h3.v1=0 | ( s_G.h3.v1=1 & s_CL.v0=1 ) )
     & (exists PrCount t_pc.
           (     Sequ_Reach( s_block, s_r, s_mod, t_pc, s_CL, s_G )
               & programInt2(s_mod,t_pc,s_pc,s_CL,s_G.h3)
           )
       )
    )


);

#size Sequ_Reach;
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
            CS         t_r,
            Module     t_mod,
            PrCount    t_pc,
            Local      t_CL,
            Globals    t_G.
        (   Sequ_Reach( t_block, t_r, t_mod, t_pc, t_CL, t_G )
          & (   target( t_mod, t_pc )
              & ( t_block=thread1 | t_block=thread2 | t_block=thread3)
            )
        )
     )


);

#timer stop;
