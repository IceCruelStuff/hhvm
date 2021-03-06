<?hh

record A {
  int x;
  string y;
}

function new_a(): A {
  return A['x' => 1, 'y' => 'stuff'];
}

function new_a_recordarray(): A {
  return A@['x' => 1, 'y' => 'stuff'];
}

// Error
function new_b(): NotFound {
  return NotFound['x' => 1, 'y' => 'stuff'];
}

function new_b_recordarray(): NotFound {
  return NotFound@['x' => 1, 'y' => 'stuff'];
}

// Do not allow generics
function new_t<reify T>(): T {
  return T['x' => 1, 'y' => 'stuff'];
}

function new_t_recordarray<reify T>(): T {
  return T@['x' => 1, 'y' => 'stuff'];
}

// Do not allow type alias

type Alias = A;

function new_alias(): Alias {
  return Alias['x' => 1, 'y' => 'stuff'];
}

function new_alias_recordarray(): Alias {
  return Alias@['x' => 1, 'y' => 'stuff'];
}
