// Event table
// To add/remove an event, edit only this table.
// No strings longer than 30 characters!
// eventnumber is computed automatically — do not set it manually.

struct Event {
  uint8_t     month;
  uint8_t     day;
  const char* description;
};

// Event table — consolidated from three source files
// To add/remove an event, edit only this table.
// No strings longer than 30 characters!
// eventnumber is computed automatically — do not set it manually.
//


const Event events[] = {
  { january,    1,  "New Years!"            },
  { february,  14,  "Valentine's Day"       },
  { march,     17,  "Happy St. Pat's Day"   },
  { july,       4,  "Happy 4th"         },
  { october,   31,  "Halloween"             },
  { december,  25,  "Christmas!"            },
  { december,  31,  "New Years Eve!"        },
};

const int eventnumber = sizeof(events) / sizeof(events[0]);