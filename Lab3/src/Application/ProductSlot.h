#ifndef PRODUCT_SLOT_H_
#define PRODUCT_SLOT_H_

#include <Platform.h>
#include <Core/Base/IBase.h>
#include <Core/Base/NoCopyBase.h>

/**
 * This class represents a single product slot, maintaining the price, the max number of
 * items in the slot, the actual number of items in the slot and the slot's state.
 */
class ProductSlot : public redBlocks::Core::NoCopyBase
{
  public:
    /**
     * Constructor
     */
    ProductSlot() : mIsActive( false )
    {
    }

    /**
     * Configures a slot with the given values.
     * @param numItems the number of total items that fit in the slot
     * @param pricePerItem the price for the item
     */
    void configure( u8 numItems, u16 pricePerItem )
    {
      mNumItems = numItems;
      mPricePerItem = pricePerItem;
    }

    /**
     * Activates / deactivates the slot. Activating it, sets the number of
     * items to the maximum number of items, i. e. this method must be called,
     * when the slot is refilled.
     * @param isActive true, if the slot is to be activated
     */
    void setActive( bool isActive )
    {
      mIsActive = isActive;
      if ( isActive )
      {
        mItemCount = mNumItems;
      }
    }

    /**
     * This method must be called, if an item has been sold and is successfully released.
     * @return the number of items left
     */
    u8 itemSold()
    {
      u8 retval = 0;
      if ( mIsActive )
      {
        if ( 0 < mItemCount )
        {
          --mItemCount;
        }
        retval = mItemCount;
      }
      return retval;
    }

    /**
     * Returns the price of the item
     * @return price per item in cent
     */
    u16 getPricePerItem() const
    {
      return mPricePerItem;
    }

    /**
     * Returns true, if the specified money entitles the customer to release the item
     * (if the slot is empty or not active, this method returns false).
     * @param money the money amount per 10 cent, i. e. value 15 means 150 cent
     * @return true, if the item can be released
     */
    bool canRelease( u8 money ) const
    {
      return ( mItemCount > 0 ) && ( money >= mPricePerItem ) && mIsActive;
    }

    /**
     * Returns true, if the slot is active
     * @return true, if the slot is active
     */
    bool getIsActive() const
    {
      return mIsActive;
    }

    /**
     * Returns the number of items left in the slot
     * @return the number of items left in the slot
     */
    u8 getItemCount() const
    {
      return mItemCount;
    }

  private:
    /** the number of total items that fit in the slot */
    u8 mNumItems;
    /** the number of items available in the slot */
    u8 mItemCount;
    /** the price per item in cent */
    u16 mPricePerItem;
    /** true, if the slot is active */
    bool mIsActive;
};

#endif // PRODUCT_SLOT_H_
