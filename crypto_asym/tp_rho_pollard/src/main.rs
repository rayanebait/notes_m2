use std::ops::Add;

use rug::{Complete, Integer};

fn f(x: Integer) -> Integer {
    x.square() + 1
}

fn rho_pollard(n: Integer) -> Integer {
    let mut x: Integer = 1.into();
    let mut y: Integer = x.clone();
    let mut d: Integer = 1.into();

    // let mut i : Integer = 0.into();
    while d == 1 {
        x = f(x) % (&n);
        y = f(f(y) % (&n)) % (&n);
        d = (&x - &y).complete().abs();
        d = n.gcd_ref(&d).into();

        // if i.mod_u(1000) == 0 {
        //     println!("{x}");
        // }
        // i=i+1;
    }

    d
}

fn primes_below(bound: Integer)-> Vec<Integer>{
    vec![]
}

fn cq_precomp(P: Integer, A: Integer){


}

fn crible_quadratique(n: Integer, P: Integer, A: Integer) -> Integer {
    cq_precomp(P, A);

    n
}

fn main() {
    let n1: Integer = 52590354472497239257283147i128.into();
    let n2: Integer = 52590354464570687296135717939981i128.into();

    println!("{}", rho_pollard(n2));
}
