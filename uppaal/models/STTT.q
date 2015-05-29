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

*/
E[] (not Pvv.err)

/*
[Heart condition 2] There exist an execution trace in which ventricular interval is no larger than TURI
*/
E[] (not Pvp_vp.err)

/*

*/
E<> Preentry.reentry

/*

*/
A<> (PMS.err)

/*

*/
E[] (not Pvv.err)

/*

*/
E<> (not Pvp_vp.err)

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
A[] (not deadlock)
