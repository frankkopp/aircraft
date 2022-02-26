/* eslint-disable max-len */
import React, { useEffect, useState } from 'react';
import { CheckLg, Link45deg } from 'react-bootstrap-icons';
import { usePersistentNumberProperty } from '@instruments/common/persistence';
import { toast } from 'react-toastify';
import { useSimVar } from '@instruments/common/simVars';
import { ScrollableContainer } from '../UtilComponents/ScrollableContainer';
import {
    areAllChecklistItemsCompleted,
    setChecklistCompletion,
    setChecklistItemCompletion,
    setSelectedChecklistIndex,
} from '../Store/features/checklists';
import { useAppDispatch, useAppSelector } from '../Store/store';
import { CHECKLISTS } from './Lists';
import { ChecklistItem } from './Checklists';

interface ChecklistItemComponentProps {
    item: ChecklistItem;
    index: number;
}

const ChecklistItemComponent = ({ item, index }: ChecklistItemComponentProps) => {
    const dispatch = useAppDispatch();
    const [checklistShake, setChecklistShake] = useState(false);
    const [autoFillChecklists] = usePersistentNumberProperty('EFB_AUTOFILL_CHECKLISTS', 0);
    const { selectedChecklistIndex, checklists } = useAppSelector((state) => state.trackingChecklists);
    const isItemCompleted = checklists[selectedChecklistIndex].items[index]?.completed;

    const firstIncompleteIdx = checklists[selectedChecklistIndex].items.findIndex((item) => {
        if (autoFillChecklists) {
            return !item.completed && !item.hasCondition;
        }

        return !item.completed;
    });

    const itemCheckedAfterIncomplete = checklists[selectedChecklistIndex].items
        .slice(firstIncompleteIdx)
        .some((item) => item.completed && (autoFillChecklists ? !item.hasCondition : true));

    const itemImproperlyUnchecked = index === firstIncompleteIdx && itemCheckedAfterIncomplete;

    let color = 'text-theme-text';

    if (isItemCompleted) {
        color = 'text-colors-lime-400';
    }

    if (itemImproperlyUnchecked) {
        color = 'text-red-500';
    }

    const [autoItemTouches, setAutoItemTouches] = useState(0);

    useEffect(() => {
        if (autoItemTouches === 5) {
            toast.info('You cannot interact with this item because you have enabled the autofill checklist option in the Realism settings page.');
            setAutoItemTouches(0);
        }
    }, [autoItemTouches]);

    return (
        <div
            className={`flex flex-row items-center py-2 space-x-4 ${color}`}
            onClick={() => {
                if (item.condition && autoFillChecklists) {
                    setAutoItemTouches((old) => old + 1);
                    setChecklistShake(true);
                    setTimeout(() => {
                        setChecklistShake(false);
                    }, 1000);
                    return;
                }

                dispatch(setChecklistItemCompletion({
                    checklistIndex: selectedChecklistIndex,
                    itemIndex: index,
                    completionValue: !isItemCompleted,
                }));

                if (isItemCompleted) {
                    dispatch(setChecklistCompletion({ checklistIndex: selectedChecklistIndex, completion: false }));
                }
            }}
        >
            {item.item && (
                <div
                    className="flex flex-shrink-0 justify-center items-center w-8 h-8 text-current border-4 border-current"
                >
                    {(!!autoFillChecklists && item.condition) && (
                        <Link45deg size={40} className={`${checklistShake && 'shake text-red-500'}`} />
                    )}
                    {(isItemCompleted && (!autoFillChecklists || (autoFillChecklists && !item.condition))) && (
                        <CheckLg size={40} />
                    )}
                </div>
            )}
            <div className="flex flex-row items-end w-full text-current">
                <div className="text-current whitespace-nowrap">
                    {item.item}
                    {isItemCompleted && ':'}
                </div>
                <div className={`h-0.5 mb-1.5 text-current bg-current ${isItemCompleted ? 'w-0 mx-2' : 'w-full mx-4'}`} />
                <div className="text-current whitespace-nowrap">
                    {item.result}
                </div>
            </div>
        </div>
    );
};

const CompletionButton = () => {
    const { selectedChecklistIndex, checklists } = useAppSelector((state) => state.trackingChecklists);
    const [autoFillChecklists] = usePersistentNumberProperty('EFB_AUTOFILL_CHECKLISTS', 0);
    const [completeItemVar, setCompleteItemVar] = useSimVar('L:A32NX_EFB_CHECKLIST_COMPLETE_ITEM', 'bool', 200);

    const firstIncompleteIdx = checklists[selectedChecklistIndex].items.findIndex((item, index) => {
        // Let's go ahead and skip checklist items that have a completion-determination function as those can't be manually checked.
        if (autoFillChecklists) {
            return !item.completed && !CHECKLISTS[selectedChecklistIndex].items[index].condition;
        }

        return !item.completed;
    });

    const dispatch = useAppDispatch();

    useEffect(() => {
        setCompleteItemVar(false);
    }, []);

    useEffect(() => {
        if (completeItemVar) {
            setCompleteItemVar(false);
            if (checklists[selectedChecklistIndex].markedCompleted && selectedChecklistIndex < checklists.length - 1) {
                dispatch(setSelectedChecklistIndex(selectedChecklistIndex + 1));
            } else if (firstIncompleteIdx !== -1) {
                dispatch(setChecklistItemCompletion({
                    checklistIndex: selectedChecklistIndex,
                    itemIndex: firstIncompleteIdx,
                    completionValue: true,
                }));
            } else if (areAllChecklistItemsCompleted(selectedChecklistIndex)) {
                dispatch(setChecklistCompletion({ checklistIndex: selectedChecklistIndex, completion: true }));
            }
        }
    }, [completeItemVar]);

    if (checklists[selectedChecklistIndex].markedCompleted) {
        if (selectedChecklistIndex < checklists.length - 1) {
            return (
                <div
                    className="flex justify-center items-center py-2 w-full text-theme-highlight hover:text-theme-body bg-theme-body hover:bg-theme-highlight rounded-md border-2 border-theme-highlight transition duration-100"
                    onClick={() => {
                        dispatch(setSelectedChecklistIndex(selectedChecklistIndex + 1));
                    }}
                >
                    Proceed to next checklist
                </div>
            );
        }

        return (
            <div className="flex justify-center items-center py-2 w-full text-theme-highlight bg-theme-body rounded-md border-2 border-theme-highlight">
                The last checklist is complete
            </div>
        );
    }

    if (firstIncompleteIdx !== -1) {
        return (
            <div
                className="flex justify-center items-center py-2 w-full hover:text-theme-body bg-theme-body rounded-md border-2 transition duration-100 border-colors-lime-400 hover:bg-colors-lime-400 text-colors-lime-400"
                onClick={() => {
                    dispatch(setChecklistItemCompletion({
                        checklistIndex: selectedChecklistIndex,
                        itemIndex: firstIncompleteIdx,
                        completionValue: true,
                    }));
                }}
            >
                Mark item as complete
            </div>
        );
    }

    if (areAllChecklistItemsCompleted(selectedChecklistIndex)) {
        return (
            <div
                className="flex justify-center items-center py-2 w-full hover:text-theme-body bg-theme-body rounded-md border-2 transition duration-100 border-colors-lime-400 hover:bg-colors-lime-400 text-colors-lime-400"
                onClick={() => {
                    dispatch(setChecklistCompletion({ checklistIndex: selectedChecklistIndex, completion: true }));
                }}
            >
                Mark checklist as complete
            </div>
        );
    }

    return (
        <div className="flex justify-center items-center py-2 w-full bg-theme-body rounded-md border-2 border-colors-lime-400 text-colors-lime-400">
            There are remaining autofill checklist items that have not yet been completed
        </div>
    );
};

export const ChecklistPage = () => {
    const { selectedChecklistIndex } = useAppSelector((state) => state.trackingChecklists);

    return (
        <div className="flex overflow-visible flex-col justify-between p-8 w-full rounded-lg border-2 border-theme-accent">
            <ScrollableContainer height={46}>
                <div className="space-y-4">
                    {CHECKLISTS[selectedChecklistIndex].items.map((it, index) => (
                        <ChecklistItemComponent item={it} index={index} />
                    ))}
                </div>
            </ScrollableContainer>

            <CompletionButton />
        </div>
    );
};
