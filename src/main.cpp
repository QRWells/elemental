#include <iostream>

#include "elements/membrane.hpp"

using namespace std;

using namespace elemental::elements;

int main(int argc, char **argv) {
  auto *m = new Membrane("root");
  m->AddAtom(new Atom(m, "a"));
  m->AddAtom(new Atom(m, "b"));
  if (auto res = m->FindAtom("a", m, 0); res.has_value()) {
    cout << "Found atom a" << endl;
    m->RemoveAtom(res.value());
  }
  cout << "Membrane size: " << m->Size() << endl;
  return 0;
}
