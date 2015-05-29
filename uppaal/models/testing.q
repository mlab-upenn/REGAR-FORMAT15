//This file was generated from (Academic) UPPAAL 4.0.9 (rev. 4407), September 2009

/*
[Heart condition 1] For URI component: A ventricle pace(VP) can only happen at least TURI after a ventricle event(VS VP)
*/
A[] (PURI_test.interval imply PURI_test.t>=TURI)

/*
[Heart condition 1] Bradycardia: the v-v interval should be no bigger than TLRI no matter how we set the heart rate for random heart. We can\u2019t say the same thing for a-a interval.
*/
A[] (Pvv.two_a imply Pvv.t<=TLRI)

/*
[Heart condition 2] There exist an execution trace in which ventricular interval is no larger than TURI
*/
E[] (not Pvp_vp.err)

/*

*/
A<> (PMS.err)

/*

*/
E<>(t1==1 && t2==1 && t3==1 && t4==1 && t5==1 && t6==1 && t7==1 && t8==1 && t9==1 && t10==1 && t11==1 && t12==1 && t13==1 && t14==1 && t15==1 && t16==1 && t17==1 && t18==1 && t19==1 && t20==1 && t21==1 && t22==1 )

/*

*/
E[] (not Pvp_vp.err && not PELT_det.err)

/*

*/
E<> Pmonitor.done

/*

*/
E<> count==20

/*

*/
E[] (not Pvp_vp.err)

/*

*/
A[] (not deadlock)
