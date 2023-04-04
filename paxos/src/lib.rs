pub mod netutil;
pub mod protocol;

use std::error;
use std::fmt;

#[derive(Debug)]
struct CustomError {
    msg: String,
}

impl CustomError {
    pub fn new(msg: &str) -> Self {
        Self {
            msg: msg.to_owned(),
        }
    }
}

impl fmt::Display for CustomError {
    fn fmt(&self, fmt: &mut fmt::Formatter) -> fmt::Result {
        write!(fmt, "{}", self.msg)
    }
}

impl error::Error for CustomError {}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test() {}
}
