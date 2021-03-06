#ifndef _BCACHEFS_JOURNAL_RECLAIM_H
#define _BCACHEFS_JOURNAL_RECLAIM_H

#define JOURNAL_PIN	(32 * 1024)

static inline bool journal_pin_active(struct journal_entry_pin *pin)
{
	return pin->pin_list != NULL;
}

static inline struct journal_entry_pin_list *
journal_seq_pin(struct journal *j, u64 seq)
{
	BUG_ON(seq < j->pin.front || seq >= j->pin.back);

	return &j->pin.data[seq & j->pin.mask];
}

u64 bch2_journal_pin_seq(struct journal *, struct journal_entry_pin *);

void bch2_journal_pin_add(struct journal *, u64, struct journal_entry_pin *,
			  journal_pin_flush_fn);
void bch2_journal_pin_drop(struct journal *, struct journal_entry_pin *);
void bch2_journal_pin_add_if_older(struct journal *,
				  struct journal_entry_pin *,
				  struct journal_entry_pin *,
				  journal_pin_flush_fn);

void bch2_journal_reclaim_fast(struct journal *);
void bch2_journal_reclaim_work(struct work_struct *);

void bch2_journal_flush_pins(struct journal *, u64);

static inline void bch2_journal_flush_all_pins(struct journal *j)
{
	bch2_journal_flush_pins(j, U64_MAX);
}

int bch2_journal_flush_device_pins(struct journal *, int);

#endif /* _BCACHEFS_JOURNAL_RECLAIM_H */
