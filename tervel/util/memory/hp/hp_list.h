#ifndef TERVEL_UTIL_MEMORY_HP_LIST_H_
#define TERVEL_UTIL_MEMORY_HP_LIST_H_

#include <atomic>
#include <utility>

#include <assert.h>
#include <stdint.h>

#include "tervel/util/descriptor.h"
#include "tervel/util/info.h"
#include "tervel/util/memory/hp/pool_element.h"
#include "tervel/util/memory/hp/pool_manager.h"
#include "tervel/util/system.h"

namespace tervel {
namespace util {
namespace memory {
namespace hp {

class Element;
class ListManager;

// REVIEW(carlos): prefix HP on the List is redundant given the namespace name
//   of hp.

/**
 * Defines a list of objects which are stored until they are safe to be freed.
 *
 * The list is represented as a linked list of HP Elements
 *
 * Further, this object has a parent who is shared amongst other threads.
 * When it is to be destroyed, it sends its remaining elements to the
 * parent, relinquishing ownership of said elements. 
 */
class HPList {
 public:
  friend Element;
  explicit HPList(ListManager *manager) : manager_(manager) {}

  ~HPList() { this->send_to_manager(); }


 private:
  // -------------------------
  // FOR DEALING WITH MANAGERS
  // -------------------------

  /**
   * Sends all elements managed by this list to the parent.
   */
  void send_to_manager();


  // --------------------------------
  // DEALS WITH UNSAFE LIST
  // --------------------------------
  /**
   * This function adds an Element to the unsafe list.
   * @param elem The element to add
   */
  void add_to_unsafe(Element* elem);

  /**
   * Tries to free elements from the unsafe list.
   * @param dont_check If true, it ignores safty checks 
   */
  void try_to_free_Elements(bool dont_check = false);


  // -------
  // MEMBERS
  // -------

  /**
   * This list's manager.
   */
  ListManager *manager_;

  /**
   * A linked list of list elements. 
   * Elements are freed when they are no longer referenced by other threads.
   */
  Element *element_list_ {nullptr}
};

}  // namespace hp
}  // namespace memory
}  // namespace util
}  // namespace tervel

#endif  // TERVEL_UTIL_MEMORY_HP_LIST_H_